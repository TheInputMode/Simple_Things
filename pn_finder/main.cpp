// DATE CREATED: Jan 22, 2024
// LAST CHANGED: Jan 22, 2024, 05:38
#include "std_lib_facilities.h"

int main()
{
	/*
		Doing this shit helps the thinking process A LOT!
	
	define variables

	loop that tests each number from 2-max (defined above)
	  filter out numbers that are divisible by 2 <-------------------- This step was skipped when I created the loop
	    loop through list of primes
		  is the current number divisible by any number in the list
	        if it is, it isn't prime
		      unset flag that prime was found (if it was set before)
			  break out of current loop
			if it isn't, it is prime
			  set flag that prime was found
		was a prime found?
		  add the current number to the list

	*/

	/*
	*	We opted for Roman-Numerals defining our loops as opposed to the usual
	*	i, j, and k variables... kinda confusing, and probably not happening again :|
	*/

	size_t end = 0;
	std::cout << "Enter end of range above zero: ";
	std::cin >> end;

	const auto time_start{ std::chrono::steady_clock::now() };

/*
*	This is the main algorithm, everything else is either printing or time-keeping (aside from input above)
*/
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	bool found_prime = false;
	std::vector<size_t> primes; // Stored in a vector. Can be written to a file or whatever, but I don't know how to do that :(
	primes.push_back(2);        // start on 2 because fuck you
	
	for (size_t I = 3; I <= end; I += 2) // this skips the check for even numbers (only example of NOT using ++ I've seen so far)
	{
		for (size_t II = 0; II < primes.size(); II++)
		{
			if (primes[II] > I / 2) {
				break;
			}
			if (I % primes[II] == 0) {
				found_prime = false;
				break;
			}
			else {
				found_prime = true;
			}
		}
		if (found_prime) {
			primes.push_back(I);
		}
	}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	const auto time_end{ std::chrono::steady_clock::now() };

	// print those primes
	for (size_t I = 0; I < primes.size(); I++)
	{
		std::cout << primes[I] << '\n';
	}

	const auto time_end_total{ std::chrono::steady_clock::now() };

	// calculate times
	const std::chrono::duration<double> elapsed_time{ time_end - time_start };
	const std::chrono::duration<double> total_time{ time_end_total - time_start };
	std::cout
		<< "Total number of primes from 0 to " << end << ": " << primes.size() << std::endl
		<< "Calculation Time: " << elapsed_time.count() << " seconds\n"
		<< "Total Time: " << total_time.count() << " seconds\n\n";

	keep_window_open();

	return 0;
}
