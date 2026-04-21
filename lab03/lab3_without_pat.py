import tkinter as tk
from tkinter import filedialog, messagebox
import json
import csv
import os


class App:
    def __init__(self, root):
        self.root = root
        self.root.title("File Processor")
        self.root.geometry("500x300")

        self.input_path = ""

        tk.Button(root, text="Выбрать файл", command=self.choose_file).pack(pady=10)
        tk.Button(root, text="Обработать", command=self.process_file).pack(pady=10)

        self.label = tk.Label(root, text="Файл не выбран")
        self.label.pack(pady=10)

    def choose_file(self):
        self.input_path = filedialog.askopenfilename()
        self.label.config(text=self.input_path)

    def process_file(self):
        if not self.input_path:

            messagebox.showerror("Ошибка", "Выберите файл")
            return

        try:
            output_path = filedialog.asksaveasfilename(defaultextension=".txt")
            if not output_path:
                return

            ext = os.path.splitext(self.input_path)[1].lower()

          

            if ext == ".txt":
                with open(self.input_path, "r", encoding="utf-8") as f:
                    data = f.readlines()

                processed = [line.strip().upper() for line in data if line.strip()]

                with open(output_path, "w", encoding="utf-8") as f:
                    f.write("\n".join(processed))

            elif ext == ".csv":
                with open(self.input_path, newline="", encoding="utf-8") as f:
                    data = list(csv.reader(f))

                processed = [[i] + row for i, row in enumerate(data)]

                with open(output_path, "w", newline="", encoding="utf-8") as f:
                    writer = csv.writer(f)
                    writer.writerows(processed)

            elif ext == ".json":
                with open(self.input_path, "r", encoding="utf-8") as f:
                    data = json.load(f)

                if isinstance(data, dict):
                    data["processed"] = True

                with open(output_path, "w", encoding="utf-8") as f:
                    json.dump(data, f, indent=4)

            else:
                raise ValueError("Неподдерживаемый формат")

            messagebox.showinfo("Успех", "Файл обработан!")

        except Exception as e:
            messagebox.showerror("Ошибка", str(e))


if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()
