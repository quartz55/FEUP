bool Juvenil::aluga(Cliente *cli1){
  if(indisponivel != 0)
    return false;
  
  else{
    indisponivel = 7;
    cliente = cli1;
    return true;
  }
}

bool Terror::aluga(Cliente *cli1){
  if(indisponivel != 0)
    return false;
    
  else{
    if(categoria == 'C)
      indisponivel = 5;
    else
      indisponivel = 3;
    
    cliente = cli1;
    return true;
  }
}
