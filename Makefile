MAKEFLAGS += --no-print-directory

.DEFAULT_GOAL := help

.PHONY: build check-pio clean help install-pio

build: check-pio ## Compile firmware
	./.make/run-pio.sh run

clean: ## Remove build artifacts
	./.make/clean.sh

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
