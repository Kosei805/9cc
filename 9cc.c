#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの種類
typedef enum { TK_RESERVED, TK_NUM, TK_EOF } TokenKind;

typedef struct Token Token;

// トークンの型
struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
};

// 現在注目しているトークン
Token *token;

// エラーを出力する関数。printfと同じ引数。
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// 入力したプログラム
char *user_input;

// エラー個所の報告
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s/n", user_input);
  fprintf(stderr, "%*s", pos, " ");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// 次のトークンが期待した通りなら真を返し、そうでないなら偽を返す。
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) return false;
  token = token->next;
  return true;
}

// 次のトークンが期待通りなら読み進める。そうでないならエラー。
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error_at(token->str, "'%c'ではありません", op);
  token = token->next;
}

// 次のトークンが数字であるなら数字を返し、そうでないならエラーを返す
int expect_number() {
  if (token->kind != TK_NUM) error_at(token->str, "数ではありません");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() { return token->kind == TK_EOF; }

// 新しいトークンを作成してcurにつなげる
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// 入力文字列pをトークナイズして返す
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // 空白文字をスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    // +か-なら加算と減算
    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    // 数字トークン
    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(token->str, "トークナイズできません");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の数が正しくありません");
    return -1;
  }

  // ユーザーの入力を保存する。
  user_input = argv[1];

  // トークナイズする
  token = tokenize(argv[1]);

  // アセンブリの前半部分を書く
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // 式の最初は数字でないといけない
  printf("\tmov rax, %d\n", expect_number());

  while (!at_eof()) {
    if (consume('+')) {
      printf("\tadd rax, %d\n", expect_number());
      continue;
    }

    expect(('-'));
    printf("\tsub rax, %d\n", expect_number());
  }

  printf("\tret\n");
  return 0;
}
