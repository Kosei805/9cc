# 9cc

- [低レイヤを知りたい人のためのCコンパイラ](https://www.sigbus.info/compilerbook)

### コンパイラ本体の作成

- コマンドライン引数をそのままreturnするプログラムのコンパイラ

### 自動テストの作成

- シンプルなテスト用のシェルスクリプト作成


### 比較演算子の追加

```
expr       = equality
equality   = relational ("==" relational | "!=" relational)*
relational = add ("<" add | "<=" add | ">" add | ">=" add)*
add        = mul ("+" mul | "-" mul)*
mul        = unary ("*" unary | "/" unary)*
unary      = ("+" | "-")? primary
primary    = num | "(" expr ")"
```