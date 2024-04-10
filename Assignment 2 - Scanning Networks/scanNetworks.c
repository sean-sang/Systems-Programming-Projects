#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define num_networks 10

typedef enum {kData, kWifi, kUnknown} MEDIUM;

typedef struct network {
  char* network_name;
  int signal_strength;
  MEDIUM connection_medium;
  bool password_saved;
} network;

network cached_networks[num_networks];

void ChooseNetwork(char* network) {
  printf("Network chosen: %s\n", network);
}

MEDIUM ConvertIntToMedium(int int_medium) {
  if (int_medium == 1) {
    return kData;
  } else if (int_medium == 2) {
    return kWifi;
  } else {
    return kUnknown;
  }
}

/**
  * TODO: This function is buggy and not complete
  *
  * We should first fix this function and then work on fixing ScanNetworksV2().
  * The fixes found in this function will help determine how to fix V2.
  **/

void ScanNetworks() {
        char* temp_name;
        int medium;
        char password_saved[10];

// fp opening experiment_data in read
        FILE *file = fopen("experiment_data", "r+");

// for loop stops at 10 networks compared to the other ScanNetworks()
        for (int i = 0; i < 10; i++) {
        temp_name = (char *)malloc(20 * sizeof(char));

        fscanf(file, "%s %d %d %s", temp_name, &medium,
            &cached_networks[i].signal_strength,
            password_saved);
        cached_networks[i].network_name = temp_name;
        cached_networks[i].connection_medium = ConvertIntToMedium(medium);
		        cached_networks[i].password_saved = strcmp(password_saved, "true") == 0;
        printf("%s\n", cached_networks[i].network_name);
   }

   fclose(file);
 }

/**
 ** This function early-exits from networks that we don't already have access
 ** to. This way we can still scan for 5 networks, but we won't display/attempt
 ** make a decision vs networks we don't have the password for.
 **
 ** TODO: This function is buggy and not complete
 **/
//////////////////////////////////////////////////////////////////////////
void ScanNetworksV2() {
  char* network_name;
  int signal_strength;
  int medium;
  char password_saved_str[6];
  bool password_saved;
  int i = 0;
  FILE *file = fopen("experiment_data", "r");


// Searches for 5 networks, if not enough networks that fit criteria of password availability
// it will early-exit with END OF FILE error.
    for (int i = 0; i < 5; i++) {
    network_name = (char *)malloc(20 * sizeof(char));
// memory allocation error as recommended
    if (network_name == NULL) {
      printf("MALLOC ERROR.\n");
      return;
    }
    if (fscanf(file, "%s %d %d %s", network_name, &medium, &signal_strength,
        password_saved_str) !=4) {
        printf("END OF FILE. \n");
        free(network_name);
        break;
}
// Only keep networks that you have PW access to, will continue networks until
// there are 5 networks that are cached that we have passwords for.
    password_saved = strcmp(password_saved_str, "true") == 0;
        if (password_saved) {
      cached_networks[i].network_name = network_name;
      cached_networks[i].connection_medium = ConvertIntToMedium(medium);
      cached_networks[i].signal_strength = signal_strength;
      cached_networks[i].password_saved = password_saved;
      printf("%s\n", cached_networks[i].network_name);
// If password is not available, it will free the network_name from memory
// it will also decrement i as recommended so it can continue cycling until it can find
// 5 networks or until there is not an available network found.
    } else {
      free(network_name);
	        i--;
    }
  }
  printf("===================\n");
  fclose(file);
}



//////////////////////////////////////////////////////////////////////////
  // If you were to iterate and modify ScanNetworksV2 to be even better,
  // what would you do? You DO NOT need to write any code, you can just
  // explain what you would do or write psuedocode.
//////////////////////////////////////////////////////////////////////////
// Added char* criteria so it will now only cache the criteria input.
// Will only cache data if data is entered, wifi if ...
void ScanNetworksV3(char* criteria) {
  char* network_name;
  int signal_strength;
  int medium;
  char password_saved_str[6];
  bool password_saved;
  int i = 0;
  MEDIUM desired_medium;
  if (strcmp(criteria, "data") == 0){
    desired_medium = kData;
  } else if (strcmp(criteria, "wifi") == 0) {
      desired_medium = kWifi;
  } else {
    desired_medium = kUnknown;
  }
  FILE *file = fopen("experiment_data", "r");
// While-loop to iterate throughout the entire file (experiment_data)
// or until num_networks is reached
  while (!feof(file) && i < num_networks) {
    network_name = (char *)malloc(20 * sizeof(char));

//  if not enough networks to satisfy while-loop, breaks out of loop.
//  will throw EOF message if not enough, else any other errors go to error reading file.
    if (fscanf(file, "%s %d %d %s", network_name, &medium, &signal_strength,
                         password_saved_str) !=4) {
      free(network_name);

        if(feof(file)) {
                printf("============\n");
                printf("END OF FILE.\n");
        break;
      } else {
                printf("============\n");
                printf("ERROR READING FILE.\n");
        break;
        }
    }
//  if issues with memory allocation as recommended.
    if (network_name == NULL) {
        printf("MALLOC ERROR.\n");
        return;
}
// Using a similar setup to ScanNetworksV2(), when it cycles through experiment_data it only caches networks with
// available passwords. However, it will now only output networks that fit the criteria.
      password_saved = strcmp(password_saved_str, "true") == 0;
    if (password_saved && (ConvertIntToMedium(medium) == desired_medium || strcmp(criteria, "greedy") == 0)) {
      cached_networks[i].network_name = network_name;
      cached_networks[i].connection_medium = ConvertIntToMedium(medium);
      cached_networks[i].signal_strength = signal_strength;
      cached_networks[i].password_saved = password_saved;
      printf("%s\n", cached_networks[i].network_name);
      i++;
    } else {
      free(network_name);
    }
  }
  fclose(file);
    if(num_networks == i) {
        printf("==========\n");
}
}
/**
  * This function selects what network we should connect to based on certain
  * criteria.
  *
  *
  * 1. We should only choose networks that we can connect to
  * 2. We should only connect to networks based on what criteria we want
  *    (i.e., Wi-Fi, Data, either).
  * 3. We should pick the network with the highest signal strength
  *
  * criteria    String denoting "wifi", "data", or "greedy"
  * return      String of best network_name
  */
//////////////////////////////////////////////////////////////////////////
char* DetermineNetwork(char* criteria) {
 MEDIUM desired_medium;


// strcmp to check criteria matchup with initial args
  if (strcmp(criteria, "data") == 0){
    desired_medium = kData;
  } else if (strcmp(criteria, "wifi") == 0) {
    desired_medium = kWifi;
  } else {
    desired_medium = kUnknown;
  }

  int max_signal_strength = -1;
  char* best_network = NULL;
// reads up to num_networks networks in the experiment_data
  for (int i = 0; i < num_networks; i++) {
   if(cached_networks[i].password_saved){
    if (strcmp(criteria, "greedy") == 0) //if greedy, will take in both wifi/data.
      {
      if (cached_networks[i].signal_strength > max_signal_strength) {
        max_signal_strength = cached_networks[i].signal_strength;
        best_network = cached_networks[i].network_name;
      }
    } else if (cached_networks[i].connection_medium == desired_medium &&
               cached_networks[i].signal_strength > max_signal_strength) {
      max_signal_strength = cached_networks[i].signal_strength;
      best_network = cached_networks[i].network_name;
    }
  }
}
// if no networks to be found (issues reading file) then it throws
// an error.
  if (best_network == NULL) {
    printf("Failed to find matching networks.\n");
  }

  return best_network;
}




 // Iterate through cached_networks to choose the best network
 // For now, return the first network's name as a placeholder
 
 int main(int argc, char *argv[]) {
// changed argc!=2 to also check strcmp for wifi, data, or greedy since ScanNetworksV3 will take in an argument now.
  if (argc != 2 || (strcmp(argv[1], "wifi") != 0 && strcmp (argv[1], "data") != 0 && strcmp (argv[1], "greedy") != 0)) {    printf("Incorrect command argument. Please pass in wifi, data, or greedy\n");
    return 1;
  }

  printf("Starting up modem...\n");
  printf("Scanning nearby network connections...\n");
  ScanNetworksV3(argv[1]);
  printf("Networks cached. Now determining network to connect...\n");
  printf("Connection Medium Criteria: %s\n", argv[1]);
  ChooseNetwork(DetermineNetwork(argv[1]));

  return 0;
}