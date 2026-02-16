#!/usr/bin/env python3
"""
Serve the heatmap visualizer with real-time keylog streaming.

Usage:
    # Terminal 1 — capture keylog:
    qmk console | tee ~/keylog.txt

    # Terminal 2 — start the visualizer:
    python3 serve_heatmap.py
"""
import http.server
import os
import sys
import time
import webbrowser

PORT = 8989
KEYLOG_PATH = os.path.expanduser("~/keylog.txt")
HEATMAP_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "heatmap.html")


class Handler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == "/":
            self._serve_file(HEATMAP_PATH, "text/html")
        elif self.path == "/api/keylog":
            if os.path.exists(KEYLOG_PATH):
                self._serve_file(KEYLOG_PATH, "text/plain", no_cache=True)
            else:
                self.send_response(204)
                self.end_headers()
        elif self.path == "/api/stream":
            self._handle_sse()
        else:
            self.send_response(404)
            self.end_headers()

    def _serve_file(self, path, content_type, no_cache=False):
        try:
            with open(path, "rb") as f:
                content = f.read()
            self.send_response(200)
            self.send_header("Content-Type", content_type)
            self.send_header("Content-Length", len(content))
            if no_cache:
                self.send_header("Cache-Control", "no-cache, no-store")
            self.end_headers()
            self.wfile.write(content)
        except FileNotFoundError:
            self.send_response(404)
            self.end_headers()

    def _handle_sse(self):
        self.send_response(200)
        self.send_header("Content-Type", "text/event-stream")
        self.send_header("Cache-Control", "no-cache")
        self.send_header("Connection", "keep-alive")
        self.send_header("X-Accel-Buffering", "no")
        self.end_headers()

        try:
            # Wait for the file to exist
            while not os.path.exists(KEYLOG_PATH):
                self.wfile.write(b": waiting for keylog\n\n")
                self.wfile.flush()
                time.sleep(1)

            with open(KEYLOG_PATH, "r") as f:
                # Seek to end — the client already loaded the full file via /api/keylog
                f.seek(0, 2)

                while True:
                    line = f.readline()
                    if line:
                        stripped = line.strip()
                        if "KD:" in stripped or "KU:" in stripped or "LL:" in stripped:
                            self.wfile.write(f"data: {stripped}\n\n".encode())
                            self.wfile.flush()
                    else:
                        time.sleep(0.05)
        except (BrokenPipeError, ConnectionResetError, OSError):
            pass  # client disconnected

    def log_message(self, format, *args):
        pass


def main():
    if not os.path.exists(HEATMAP_PATH):
        print(f"Error: {HEATMAP_PATH} not found")
        sys.exit(1)

    http.server.ThreadingHTTPServer.allow_reuse_address = True
    server = http.server.ThreadingHTTPServer(("localhost", PORT), Handler)
    url = f"http://localhost:{PORT}"
    print(f"Heatmap:  {url}")
    print(f"Keylog:   {KEYLOG_PATH}")
    if os.path.exists(KEYLOG_PATH):
        size = os.path.getsize(KEYLOG_PATH)
        print(f"          ({size:,} bytes)")
    else:
        print(f"          (waiting for file)")
    print(f"Ctrl+C to stop\n")
    webbrowser.open(url)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nStopped.")
        server.server_close()


if __name__ == "__main__":
    main()
