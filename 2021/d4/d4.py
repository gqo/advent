class BingoCell:
  def __init__(self, val):
    self.val = int(val)
    self.state = False

  def __str__(self):
    return "Cell(" + str(self.val) + "," + str(self.state) + ")"

  def __repr__(self):
    return self.__str__()

  def mark(self):
    self.state = True

class BingoBoard:
  def __init__(self, board):
    self.board = board
    self.won = False

  def __str__(self):
    res = "--- BingoBoard ---\n"
    for row in self.board:
      for cell in row:
        res += str(cell) + " "
        if cell.val < 10:
          res += " "
      res += "\n"

    return res

  def __repr__(self):
    return self.__str__()
  
  def mark_cells(self, bingo_val):
    for row in self.board:
      for cell in row:
        if cell.val == bingo_val:
          cell.mark()


  def is_bingo(self):
    flat_board = [cell for row in self.board for cell in row]

    # Diagonals don't count :(
    # # Check upper left to bottom right
    # if all([cell.state for cell in flat_board[::6]]): return True

    # # Check upper right to bottom left
    # if all([cell.state for cell in flat_board[4:21:4]]): return True

    # Check if any row is bingo
    for i in range(0,24,5):
      if all([cell.state for cell in flat_board[i:i+5]]):
        self.won = True
        return True

    # Check if any column is bingo
    for i in range(0,5):
      if all([cell.state for cell in flat_board[i::5]]): 
        self.won = True
        return True
    
    # No bingos :(
    return False

  def sum_unmarked(self):
    acc = 0

    for row in self.board:
      for cell in row:
        if not cell.state:
          acc += cell.val

    return acc

class Bingo:
  def __init__(self, nums, lines):
    self.nums = nums

    self.boards = []

    for i in range(0, len(lines) - 1):
      if lines[i] == "\n":
        board = []

        for line in lines[i+1:i+6]:
          row = [BingoCell(cell) for cell in line.strip("\n").split()]
          board.append(row)
          i += 5

        self.boards.append(BingoBoard(board))

  def __str__(self):
    res = ""
    for board in self.boards:
      res += str(board) + "\n"

    return res

  def run_part1(self):
    for num in self.nums:
      for board in self.boards:
        board.mark_cells(int(num))
        if board.is_bingo():
          return int(num) * board.sum_unmarked()

  # Keep track of the last winning board and only continue to mark boards that haven't won yet
  def run_part2(self):
    for num in self.nums:
      for board in [board for board in self.boards if not board.won]:
        board.mark_cells(int(num))
        if board.is_bingo():
          self.last_win = (board, num)

    return int(self.last_win[1]) * self.last_win[0].sum_unmarked()

def main():
  f = open("input.txt")

  nums = f.readline().strip().split(",")
  lines = f.readlines()

  f.close()

  b = Bingo(nums, lines)

  print("Answer Part 1:", b.run_part1())
  print("Answer Part 2:", b.run_part2())

main()