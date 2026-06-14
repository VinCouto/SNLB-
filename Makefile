CC = gcc

# Caminhos do Likwid (Padrão dos laboratórios do DInf, mude se necessário)
LIKWID_HOME = /home/soft/likwid
LIKWID_INC  = $(LIKWID_HOME)/include
LIKWID_LIB  = $(LIKWID_HOME)/lib

# Flags de Compilação exigidas pelo enunciado
# -DLIKWID_PERFMON habilita a API de marcadores do LIKWID no código
CFLAGS = -O3 -march=native -mavx -fopt-info-vec -DLIKWID_PERFMON -I$(LIKWID_INC)
LFLAGS = -L$(LIKWID_LIB) -llikwid -lm

# Nome do executável exigido obrigatoriamente pelo professor
PROG = ./broyden

# Arquivo que contém a função main()
MAIN_SRC = broyden.c

# Nossos módulos isolados
MODULES = utils jacobiana sistema_linear newton

# Geração automática da lista de objetos (.o)
OBJS = $(MODULES:=.o) $(MAIN_SRC:.c=.o)

.PHONY: clean all

# Regra OBRIGATÓRIA: all
all: $(PROG)

# Regra genérica para compilar os módulos individualmente (.c -> .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra de linkagem final para gerar o executável broyden
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

# Regra OBRIGATÓRIA: clean
clean:
	rm -f *.o $(PROG)
