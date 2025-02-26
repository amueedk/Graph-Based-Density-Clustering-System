import subprocess
import threading
import tkinter as tk
from tkinter import filedialog, messagebox

def run_clustering(mode):
    filename = file_entry.get()
    density = density_entry.get()
    cp = cp_entry.get()

    if not filename or not density or not cp:
        messagebox.showerror("Error", "Please enter all fields")
        return

    try:
        density = float(density)
        cp = float(cp)
    except ValueError:
        messagebox.showerror("Error", "Thresholds must be numbers")
        return

    threading.Thread(target=execute_clustering, args=(filename, density, cp, mode), daemon=True).start()

def execute_clustering(filename, density, cp, mode):
    try:
        process = subprocess.Popen(
            ["./graph_clustering.exe", filename, str(density), str(cp), mode],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        output_text.delete("1.0", tk.END)

        for line in iter(process.stdout.readline, ''):
            output_text.insert(tk.END, line)
            output_text.see(tk.END)
            root.update_idletasks()

        process.stdout.close()
        process.wait()

    except Exception as e:
        messagebox.showerror("Error", f"Clustering failed:\n{str(e)}")

def select_file():
    filename = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt"), ("All Files", "*.*")])
    file_entry.delete(0, tk.END)
    file_entry.insert(0, filename)

root = tk.Tk()
root.title("Graph Clustering UI")

tk.Label(root, text="Graph File:").grid(row=0, column=0, padx=5, pady=5)
file_entry = tk.Entry(root, width=40)
file_entry.grid(row=0, column=1, padx=5, pady=5)
tk.Button(root, text="Browse", command=select_file).grid(row=0, column=2, padx=5, pady=5)

tk.Label(root, text="Density Threshold:").grid(row=1, column=0, padx=5, pady=5)
density_entry = tk.Entry(root)
density_entry.grid(row=1, column=1, padx=5, pady=5)

tk.Label(root, text="CP Threshold:").grid(row=2, column=0, padx=5, pady=5)
cp_entry = tk.Entry(root)
cp_entry.grid(row=2, column=1, padx=5, pady=5)

tk.Button(root, text="View Statistics", command=lambda: run_clustering("stats")).grid(row=3, column=0, pady=10)
tk.Button(root, text="View Cluster Nodes", command=lambda: run_clustering("clusters")).grid(row=3, column=1, pady=10)

output_text = tk.Text(root, width=60, height=15)
output_text.grid(row=4, column=0, columnspan=3, padx=5, pady=5)

root.mainloop()
