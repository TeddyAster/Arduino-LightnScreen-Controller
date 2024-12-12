import tkinter as tk

class EmotionPainter:
    def __init__(self, root):
        self.root = root
        self.root.title("Emoticon Painter (WS2812B)")
        self.rows = 8
        self.cols = 32
        self.matrix = [[0 for _ in range(self.cols)] for _ in range(self.rows)]

        # 创建Canvas
        self.cell_size = 20
        self.canvas = tk.Canvas(root, width=self.cols * self.cell_size, height=self.rows * self.cell_size)
        self.canvas.grid(row=0, column=0, padx=10, pady=10)
        self.cells = {}

        # 初始化点阵方块
        for row in range(self.rows):
            for col in range(self.cols):
                x1 = col * self.cell_size
                y1 = row * self.cell_size
                x2 = x1 + self.cell_size
                y2 = y1 + self.cell_size
                rect = self.canvas.create_rectangle(x1, y1, x2, y2, fill="white", outline="black")
                self.cells[(row, col)] = rect
        self.canvas.bind("<Button-1>", self.toggle_cell)

        # 显示数组
        self.result_text = tk.Text(root, height=10, width=40, wrap="none")
        self.result_text.grid(row=1, column=0, padx=10, pady=10)
        self.result_text.config(state="disabled")  # 初始只读

        # 更新点阵显示
        self.update_result()

    def toggle_cell(self, event):
        # 获取鼠标点击的方块位置
        col = event.x // self.cell_size
        row = event.y // self.cell_size
        if 0 <= col < self.cols and 0 <= row < self.rows:
            # 切换方块状态
            self.matrix[row][col] = 1 - self.matrix[row][col]
            color = "black" if self.matrix[row][col] == 1 else "white"
            self.canvas.itemconfig(self.cells[(row, col)], fill=color)
            self.update_result()

    def update_result(self):
        # 将点阵矩阵转为列优先的二进制数字组
        columns = []
        for col in range(self.cols):
            column_bits = [self.matrix[row][col] for row in range(self.rows)]
            column_value = int("".join(map(str, column_bits[::-1])), 2)
            columns.append(f"{column_value:08b}")

        # 更新显示区域
        result = '\n'.join(f'"{col}"' for col in columns)
        self.result_text.config(state="normal")
        self.result_text.delete("1.0", tk.END)
        self.result_text.insert(tk.END, result)
        self.result_text.config(state="disabled")

if __name__ == "__main__":
    root = tk.Tk()
    app = EmotionPainter(root)
    root.mainloop()