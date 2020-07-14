/*
 * Copyright (c) 2018 IOTA Stiftung
 * https:github.com/iotaledger/entangled
 *
 * MAM is based on an original implementation & specification by apmi.bsu.by
 * [ITSec Lab]
 *
 * Refer to the LICENSE file for licensing information
 */

#include <stdio.h>
#include <time.h>
#include "mam/examples/send-common.h"

#define HOST "node1.puyuma.org"
#define PORT 14265
#define SEED "TFKQZVPZVWLXBJGNEPPVZNZYJFFPDMEQGGDPGSRMNXAURIELGLUCSSPGDGEQQFANGOWVXPUHNI" \
  "DOZ9999"
#define PAYLOAD "ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss"
#define LAST_PACKET "yes"

int main(int ac, char **av) {
  mam_api_t api;
  bundle_transactions_t *bundle = NULL;
  tryte_t channel_id[MAM_CHANNEL_ID_TRYTE_SIZE];
  clock_t start, end;
  float seconds;
  retcode_t ret = RC_OK;
  
  // Writing packet to bundle
  bool last_packet = true;
  
  start = clock();
  if ((ret = mam_api_init(&api, (tryte_t *)SEED)) != RC_OK) {
    fprintf(stderr, "mam_api_init failed with err %d\n", ret);
    return EXIT_FAILURE;
  }
  end = clock();
  seconds = (float)(end - start) / CLOCKS_PER_SEC;
  printf("mam_api_init = %f\n", seconds);


  start = clock();
  // Creating channel
  if ((ret = mam_example_channel_create(&api, channel_id)) != RC_OK) {
    fprintf(stderr, "mam_example_channel_create failed with err %d\n", ret);
    return EXIT_FAILURE;
  }
  end = clock();
  seconds = (float)(end - start) / CLOCKS_PER_SEC;
  printf("mam_example_channel_create = %f\n", seconds);
  
  
  start = clock();
  bundle_transactions_new(&bundle);
  {
    trit_t msg_id[MAM_MSG_ID_SIZE];

    // Writing header to bundle
    if ((ret = mam_example_write_header_on_channel(&api, channel_id, bundle, msg_id)) != RC_OK) {
      fprintf(stderr, "mam_example_write_header failed with err %d\n", ret);
      return EXIT_FAILURE;
    }
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("mam_example_write_header_on_channel = %f\n", seconds);


    start = clock();
    if ((ret = mam_example_write_packet(&api, bundle, PAYLOAD, msg_id, last_packet)) != RC_OK) {
      fprintf(stderr, "mam_example_write_packet failed with err %d\n", ret);
      return EXIT_FAILURE;
    }
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("mam_example_write_packet = %f\n", seconds);
  }

  start = clock();
  // Sending bundle
  if ((ret = send_bundle(HOST, PORT, bundle)) != RC_OK) {
    fprintf(stderr, "send_bundle failed with err %d\n", ret);
    return EXIT_FAILURE;
  }
  end = clock();
  seconds = (float)(end - start) / CLOCKS_PER_SEC;
  printf("send_bundle = %f\n", seconds);


  start = clock();
  // Saving and destroying MAM API
  if ((ret = mam_api_destroy(&api)) != RC_OK) {
    fprintf(stderr, "mam_api_destroy failed with err %d\n", ret);
    return EXIT_FAILURE;
  }
  end = clock();
  seconds = (float)(end - start) / CLOCKS_PER_SEC;
  printf("mam_api_destroy = %f\n", seconds);

  // Cleanup
  { bundle_transactions_free(&bundle); }

  return EXIT_SUCCESS;
}
