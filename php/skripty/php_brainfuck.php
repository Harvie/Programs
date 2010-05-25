<?php

$code = "++++++++[>+++++++++<-]>.<+++++[>++++++<-]>-.+++++++..+++.<++++++++[>>++++<<-]>>.<<++++[>------<-]>.<++++[>++++++<-]>.+++.------.--------.>+.";

$buffer = "";
$pos = 0;
$while = array(); // array($i, ...)
for ($i=0; $i < strlen($code); $i++) {
    switch ($code{$i}) {
        case '>': $pos++; break;
        case '<': $pos--; break;
        case '+': $buffer{$pos} = chr(ord($buffer{$pos}) + 1); break;
        case '-': $buffer{$pos} = chr(ord($buffer{$pos}) - 1); break;
        case '.': echo $buffer{$pos}; break;
        case ',': $buffer{$pos} = fgetc(STDIN); break;
        case '[': if (ord($buffer{$pos})) $while[] = $i; else for ($depth=1; $depth; $i++) {
            if ($code{$i+1} == '[') $depth++;
            elseif ($code{$i+1} == ']') $depth--;
        }
        break;
        case ']': $i = array_pop($while) - 1; break;
        default: echo "Unrecognized character.\n"; exit(1);
    }
}

echo("\n\n");

?>
