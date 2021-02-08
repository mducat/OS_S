
    .global irq0_caller
    .global irq1_caller
    .global irq2_caller
    .global irq12_caller

    .extern irq0_handler
    .extern irq1_handler
    .extern irq2_handler
    .extern irq12_handler

    
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
