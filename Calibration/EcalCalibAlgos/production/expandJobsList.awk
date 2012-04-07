{
  search=","
  n=split($0,jobs,search);
  for (i=1;i<=n;i++)
    {
      search="-";
      n1=split(jobs[i],jobLimits,search);
      if (n1 == 1)
	{
	  printf("%d,",jobs[i]); 
	}
      else if (n1 == 2)
	{
	  for (j=jobLimits[1];j<=jobLimits[2];j++)
	    {
	      printf("%d,",j); 
	    }
	}
    }
  printf("\n");
  exit;
}