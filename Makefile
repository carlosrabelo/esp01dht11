MAKEFLAGS += --no-print-directory

.DEFAULT_GOAL := help

-include .env

UPLOAD_PORT      ?= /dev/ttyUSB0
MONITOR_PORT     ?= /dev/ttyUSB0
MONITOR_SPEED    ?= 115200
UPLOAD_SPEED     ?= 115200

.PHONY: build check check-pio clean deps erase flash help install-pio monitor test upload

build: check-pio ## Compile firmware
	./.make/run-pio.sh run

upload: check-pio ## Upload firmware to device
	./.make/run-pio.sh run --target upload \
	    --upload-port $(UPLOAD_PORT) \
	    --upload-speed $(UPLOAD_SPEED)

flash: build upload ## Compile and upload

monitor: check-pio ## Open serial monitor
	./.make/run-pio.sh device monitor \
	    --port $(MONITOR_PORT) \
	    --baud $(MONITOR_SPEED)

clean: ## Remove build artifacts
	./.make/clean.sh

deps: check-pio ## Install dependencies
	./.make/run-pio.sh pkg install

check: check-pio ## Run static analysis
	./.make/run-pio.sh check

test: check-pio ## Run unit tests
	./.make/run-pio.sh test

erase: check-pio ## Erase device flash memory
	./.make/run-pio.sh run --target erase

install-pio: ## Install PlatformIO
	@./.make/install-pio.sh

check-pio: ## Verify PlatformIO is installed
	@./.make/check-pio.sh

help: ## Show available targets
	@echo "esp01dht11 - Available targets"
	@echo ""
	@grep -hE '^[a-zA-Z_-]+:.*## ' $(MAKEFILE_LIST) \
		| sort \
		| awk 'BEGIN {FS = ":.*## "} {printf "  %-15s %s\n", $$1, $$2}'
