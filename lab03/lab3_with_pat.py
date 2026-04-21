import tkinter as tk
from tkinter import filedialog, messagebox
from tkinterweb import HtmlFrame
import os
import mammoth
import fitz  


class FileProcessor:
    def process(self, path, app):
        self.open_file(path)
        self.extract_content()
        self.display(app)

    def open_file(self, path):
        raise NotImplementedError

    def extract_content(self):
        raise NotImplementedError

    def display(self, app):
        raise NotImplementedError


class TxtProcessor(FileProcessor):
    def open_file(self, path):
        with open(path, "r", encoding="utf-8") as f:
            self.text = f.read()

    def extract_content(self):
        self.html = f"<pre>{self.text}</pre>"

    def display(self, app):
        app.show_html(self.html)

class DocxProcessor(FileProcessor):
    def open_file(self, path):
        self.path = path

    def extract_content(self):
        with open(self.path, "rb") as docx_file:
            result = mammoth.convert_to_html(docx_file)
            self.html = result.value

    def display(self, app):
        app.show_html(self.html)

class PdfProcessor(FileProcessor):
    def open_file(self, path):
        self.doc = fitz.open(path)

    def extract_content(self):
        html = ""
        for page in self.doc:
            html += page.get_text("html")
        self.html = html

    def display(self, app):
        app.show_html(self.html)

class App:
    def __init__(self, root):
        self.root = root
        self.root.title("Document Viewer")
        self.root.geometry("900x600")

        top_frame = tk.Frame(root)
        top_frame.pack(fill="x")

        tk.Button(top_frame, text="Открыть файл", command=self.open_file).pack(side="left")

        self.frame = HtmlFrame(root)
        self.frame.pack(fill="both", expand=True)

    def wrap_html(self, html):
        return f"""
        <html>
        <head>
        <style>
            body {{
                font-family: Arial, sans-serif;
                padding: 10px;
            }}
            p {{ margin: 6px 0; }}
            img {{ max-width: 100%; height: auto; }}
        </style>
        </head>
        <body>
        {html}
        </body>
        </html>
        """

    def show_html(self, html):
        final_html = self.wrap_html(html)

        if hasattr(self.frame, "load_html"):
            self.frame.load_html(final_html)
        elif hasattr(self.frame, "set_content"):
            self.frame.set_content(final_html)
        else:
            messagebox.showerror("Ошибка", "tkinterweb не поддерживается")

    def open_file(self):
        path = filedialog.askopenfilename()
        if not path:
            return

        ext = os.path.splitext(path)[1].lower()

        if ext == ".txt":
            processor = TxtProcessor()
        elif ext == ".docx":
            processor = DocxProcessor()
        elif ext == ".pdf":
            processor = PdfProcessor()
        else:
            messagebox.showerror("Ошибка", "Неподдерживаемый формат")
            return

        try:
            processor.process(path, self)
        except Exception as e:
            messagebox.showerror("Ошибка", str(e))


if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()