module dcmotor(angVal,i)
	output angVal, i;

 	parameter
		K= 0.01,
		J= 0.01,
		L= 0.5,
		R= 1,
		b= 0.1;

	mode loc
		begin
   		ddt angVal = (K/J)*i - (b/J)*angVal;
   		ddt i = (-K/L)*angVal - (R/L)*i + (voltage/L);
		end 

	initial begin 
		set begin
			mode == loc;
			angVal==0;
			i==0;
			voltage==0;
		end
	end

endmodule	
