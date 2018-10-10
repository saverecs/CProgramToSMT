module powertrain(p_engine_speed, p_throttle_angle, p_throttle_flow, p_manifold_pressure, p_airbyfuel_meas, in_thetaI, c_commanded_fuel_gps)
	output p_engine_speed,c_commanded_fuel_gps;

 	mode loc
		begin
			ddt p_engine_speed = 0;
			ddt p_throttle_angle  = (10 * (in_thetaI - p_throttle_angle));
			ddt p_throttle_flow = (2 * ((2.821 + ((0-0.05231)*p_throttle_angle) + ((0.10299)*p_throttle_angle*p_throttle_angle)) + ((0-0.00063)*p_throttle_angle*p_throttle_angle*p_throttle_angle)) * ( ((0-2.3421)*p_manifold_pressure*p_manifold_pressure)+(2.7799*p_manifold_pressure)+(0-0.3273))) ;
  			ddt p_manifold_pressure      = 0.41328 * ((2 * (2.821 + ((0-0.05231)*p_throttle_angle) + (0.10299*p_throttle_angle*p_throttle_angle) + ((0-0.00063)*p_throttle_angle*p_throttle_angle*p_throttle_angle)) * ( ((0-2.3421)*p_manifold_pressure*p_manifold_pressure) + (2.7799*p_manifold_pressure) + (0-0.3273) )) - ( 0.9 * ( (0-0.366) + (0.08979*p_engine_speed*p_manifold_pressure) + ((0-0.0337)*p_engine_speed*p_manifold_pressure*p_manifold_pressure) + (0.0001*p_engine_speed*p_engine_speed*p_manifold_pressure) )));
  			ddt p_airbyfuel_meas = 4.0 * ( ( 13.893 + ((0-35.2518)*1.0*c_commanded_fuel_gps) + (20.7364*1.0*1.0*c_commanded_fuel_gps*c_commanded_fuel_gps) + (2.6287*(0.9 * ((0-0.366) + (0.08979*p_engine_speed*p_manifold_pressure) + ((0-0.0337)*p_engine_speed*p_manifold_pressure*p_manifold_pressure) + (0.0001*p_engine_speed*p_engine_speed*p_manifold_pressure)))) + ((0-1.592)*(0.9 * ((0-0.366) + (0.08979*p_engine_speed*p_manifold_pressure) + ((0-0.0337)*p_engine_speed*p_manifold_pressure*p_manifold_pressure) + (0.0001*p_engine_speed*p_engine_speed*p_manifold_pressure) ))*1.0*c_commanded_fuel_gps) ) - (p_airbyfuel_meas) );
			ddt in_thetaI = 0;
			ddt c_commanded_fuel_gps = 0;
		end 

	initial begin 
		set begin
			mode == loc;
			p_engine_speed == 95;
			p_throttle_angle==8.8;
			p_throttle_flow == 4.2;
			p_manifold_pressure==0.9833;
			p_airbyfuel_meas==14.7;
			in_thetaI == 10;
			c_commanded_fuel_gps ==0.6537 ;
		end
	end

endmodule	
