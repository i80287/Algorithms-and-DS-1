program AntineutrinoRocket;

type
  TAction = record
    x: longint;
    y: longint;
    t: longint;
  end;

  TTask = record
    test: longint;
    kMaxN: longint;
    kMaxM: longint;
    wmax: longint;
    p: array [0 .. 4] of longint;
  end;

procedure ReadData(var task: TTask; var f: text);
var
  i: longint;
begin
  ReadLn(f, task.test);
  ReadLn(f, task.kMaxN, task.kMaxM, task.wmax);
  for i := 0 to 4 do begin
    Read(f, task.p[i]);
  end;
  ReadLn(f);
end;

procedure ReadData(var task: TTask);
begin
  ReadData(task, input);
end;

procedure ReadData(var task: TTask; const filename: ansistring);
var
  f: text;
begin
  Assign(f, filename);
  Reset(f);
  ReadData(task, f);
  Close(f);
end;

// Returns -1 if sequence of actions is invalid or too big
function Evalulate(const task: TTask; const solution: array of TAction;
                   len: longint): longint;
const
  dx: array [0 .. 3] of longint = (1, -1, 0, 0);
  dy: array [0 .. 3] of longint = (0, 0, 1, -1);
var
  a: array of array of longint;
  i, j: longint;
  used: array [0 .. 4] of boolean;
  action: TAction;
  nx, ny: longint;
  score: longint = 0;
begin
  if len > 500000 then begin
    exit(-1);
  end;
  SetLength(a, task.kMaxN);
  for i := 0 to task.kMaxN - 1 do begin
    SetLength(a[i], task.kMaxM);
    for j := 0 to task.kMaxM - 1 do begin
      a[i, j] := -1;
    end;
  end;
  for i := 0 to len - 1 do begin
    action := solution[i];
    if (action.x <= 0) or (action.x > task.kMaxN) or
       (action.y <= 0) or (action.x > task.kMaxM) or
       (not (action.t in [0 .. 4])) then begin
      exit(-1);
    end;
    for j := 0 to 4 do begin
      used[j] := false;
    end;
    for j := 0 to 3 do begin
      nx := action.x - 1 + dx[j];
      ny := action.y - 1 + dy[j];
      if (nx < 0) or (nx >= task.kMaxN) or (ny < 0) or (ny >= task.kMaxM) then begin
        continue;
      end;
      if a[nx, ny] < 0 then begin
        continue;
      end;
      used[a[nx, ny]] := true;
    end;
    for j := 0 to action.t - 1 do begin
      if not used[j] then begin
        exit(-1);
      end;
    end;
    a[action.x - 1, action.y - 1] := action.t;
  end;
  for i := 0 to task.kMaxN - 1 do begin
    for j := 0 to task.kMaxM - 1 do begin
      if a[i, j] >= 0 then begin
        Inc(score, task.p[a[i, j]]);
      end;
    end;
  end;
  Evalulate := score;
end;

function Score(const task: TTask; w: longint): double;
begin
  if w <= 0 then begin
    exit(0.0);
  end;
  if w >= task.wmax then begin
    exit(10.0);
  end;
  Score := 10.0 * sqr(double(w) / task.wmax);
end;

// Prints solution and returns it's points on test
function PrintSolution(const task: TTask; const solution: array of TAction;
                       len: longint; var f: text): double;
var
  w, i: longint;
  action: TAction;
begin
  w := Evalulate(task, solution, len);
  writeLn(f, w, ' ', len);
  for i := 0 to len - 1 do begin
    action := solution[i];
    writeLn(f, action.x, ' ', action.y, ' ', action.t);
  end;
  PrintSolution := Score(task, w);
end;

// Prints solution and returns it's points on test
function PrintSolution(const task: TTask; const solution: array of TAction;
                       len: longint): double;
begin
  PrintSolution := PrintSolution(task, solution, len, output);
end;

// Prints solution and returns it's points on test
function PrintSolution(const task: TTask; const solution: array of TAction;
                       len: longint; const filename: ansistring): double;
var
  f: text;
begin
  Assign(f, filename);
  Rewrite(f);
  PrintSolution := PrintSolution(task, solution, len, f);
  Close(f);
end;

var
  totalPoints: double = 0;
  test: longint;
  inputName, outputName: ansistring;
  task: TTask;
  solution: array [0 .. 499999] of TAction;
  len: longint;
  i, j: longint;
  testPoints: double;
begin
  for test := 1 to 10 do begin
    WriteStr(inputName, 'tests/input', test, '.txt');
    WriteStr(outputName, 'output', test, '.txt');
    ReadData(task, inputName);
    len := 0;
    for i := 1 to task.kMaxN do begin
      for j := 1 to task.kMaxM do begin
        solution[len].x := i;
        solution[len].y := j;
        solution[len].t := 0;
        Inc(len);
      end;
    end;
    testPoints := PrintSolution(task, solution, len, outputName);
    WriteLn('Test ', test, ': ', testPoints : 0 : 4, ' points');
    totalPoints := totalPoints + testPoints;
  end;
  WriteLn('Total: ', totalPoints : 0 : 4, ' points');
end.