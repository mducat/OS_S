
    .global irq0_caller
    .global irq1_caller
    .global irq2_caller
    .global irq12_caller
    .global irq80_caller

    .extern irq0_handler
    .extern irq1_handler
    .extern irq2_handler
    .extern irq12_handler
    .extern irq80_handler

    
irq0_caller:    
    call    irq0_handler
    iretq

irq1_caller:    
    call    irq1_handler
    iretq

irq2_caller:
    call    irq2_handler
    iretq

irq12_caller:    
    call    irq12_handler
    iretq

irq80_caller:    
    call    irq80_handler
    iretq
