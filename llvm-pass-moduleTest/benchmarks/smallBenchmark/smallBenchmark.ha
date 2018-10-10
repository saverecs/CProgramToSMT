module smallTest(x,u)
	output x,u;

	mode loc
		begin
   		ddt x = x + 1 + u;
		end

	initial begin 
		set begin
			mode == loc;
			x == 1;
			u == 5;
		end
	end

endmodule	
