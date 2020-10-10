INFO_DIR = ./info_sys
PER_DIR = ./per_sys
PROC_DIR = ./proc

export DIR=$(PWD)

all:
	make -C $(INFO_DIR)
	make -C $(PER_DIR)
	make -C $(PROC_DIR)

clean:
	make -C $(INFO_DIR) clean
	make -C $(PER_DIR) clean
	make -C $(PROC_DIR) clean

