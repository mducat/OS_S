
    .global irq1_caller
    .global irq12_caller

    .extern irq1_handler
    .extern irq12_handler

irq1_caller:    
    call    irq1_handler
    iretq

irq12_caller:    
    call    irq12_handler
    iretq
