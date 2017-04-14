.PHONY: clean All

All:
	@echo "----------Building project:[ Synacor - Debug ]----------"
	@cd "Synacor" && "$(MAKE)" -f  "Synacor.mk"
clean:
	@echo "----------Cleaning project:[ Synacor - Debug ]----------"
	@cd "Synacor" && "$(MAKE)" -f  "Synacor.mk" clean
