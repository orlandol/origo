
  CPU 386

  BITS 32

segment .text use32

  ;
  ; void ExitProcess( UINT uExitCode )
  ;
  extern ExitProcess
  import ExitProcess kernel32.dll

run:
..start:
  push    dword 0
  call    [ExitProcess]
