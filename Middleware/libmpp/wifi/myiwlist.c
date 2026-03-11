/*
 *	Wireless Tools
 *
 *		Jean II - HPLB '99 - HPL 99->07
 *
 * This tool can access various piece of information on the card
 * not part of iwconfig...
 * You need to link this code against "iwlist.c" and "-lm".
 *
 * This file is released under the GPL license.
 *     Copyright (c) 1997-2007 Jean Tourrilhes <jt@hpl.hp.com>
 */

#include "iwlib.h"		/* Header */
#include "iwlist.h"
#include <sys/time.h>

/****************************** TYPES ******************************/

/*
 * Scan state and meta-information, used to decode events...
 */
typedef struct iwscan_state
{
  /* State */
  int			ap_num;		/* Access Point number 1->N */
  int			val_index;	/* Value in table 0->(N-1) */
} iwscan_state;

/**************************** CONSTANTS ****************************/

#define IW_SCAN_HACK		0x8000

/* ------------------------ WPA CAPA NAMES ------------------------ */
/*
 * This is the user readable name of a bunch of WPA constants in wireless.h
 * Maybe this should go in iwlib.c ?
 */

#ifndef WE_ESSENTIAL
#define IW_ARRAY_LEN(x) (sizeof(x)/sizeof((x)[0]))

#ifndef IW_IE_CIPHER_NONE
/* Cypher values in GENIE (pairwise and group) */
#define IW_IE_CIPHER_NONE	0
#define IW_IE_CIPHER_WEP40	1
#define IW_IE_CIPHER_TKIP	2
#define IW_IE_CIPHER_WRAP	3
#define IW_IE_CIPHER_CCMP	4
#define IW_IE_CIPHER_WEP104	5
/* Key management in GENIE */
#define IW_IE_KEY_MGMT_NONE	0
#define IW_IE_KEY_MGMT_802_1X	1
#define IW_IE_KEY_MGMT_PSK	2
#endif	/* IW_IE_CIPHER_NONE */

/* Values for the IW_IE_CIPHER_* in GENIE */
static const char *	iw_ie_cypher_name[] = {
	"none",
	"WEP-40",
	"TKIP",
	"WRAP",
	"CCMP",
	"WEP-104",
};
#define	IW_IE_CYPHER_NUM	IW_ARRAY_LEN(iw_ie_cypher_name)

/* Values for the IW_IE_KEY_MGMT_* in GENIE */
static const char *	iw_ie_key_mgmt_name[] = {
	"none",
	"802.1x",
	"PSK",
};
#define	IW_IE_KEY_MGMT_NUM	IW_ARRAY_LEN(iw_ie_key_mgmt_name)

#endif	/* WE_ESSENTIAL */

/************************* WPA SUBROUTINES *************************/

#ifndef WE_ESSENTIAL
/*------------------------------------------------------------------*/
/*
 * Print the name corresponding to a value, with overflow check.
 */
static void
iw_print_value_name(unsigned int		value,
		    const char *		names[],
		    const unsigned int		num_names)
{
  if(value >= num_names)
    printf(" unknown (%d)", value);
  else
    printf(" %s", names[value]);
}

/*------------------------------------------------------------------*/
/*
 * Parse, and display the results of an unknown IE.
 *
 */
static void 
iw_print_ie_unknown(unsigned char *	iebuf,
		    int			buflen)
{
  int	ielen = iebuf[1] + 2;
  int	i;

  if(ielen > buflen)
    ielen = buflen;

  printf("Unknown: ");
  for(i = 0; i < ielen; i++)
    printf("%02X", iebuf[i]);
  printf("\n");
}

/*------------------------------------------------------------------*/
/*
 * Parse, and display the results of a WPA or WPA2 IE.
 *
 */
static inline void iw_print_ie_wpa(unsigned char *	iebuf,int buflen, router_list_s *router_list)
{
    int			ielen = iebuf[1] + 2;
    int			offset = 2;	/* Skip the IE id, and the length. */
    unsigned char		wpa1_oui[3] = {0x00, 0x50, 0xf2};
    unsigned char		wpa2_oui[3] = {0x00, 0x0f, 0xac};
    unsigned char *	wpa_oui;
    int			i;
    uint16_t		ver = 0;
    uint16_t		cnt = 0;

    if(ielen > buflen)
        ielen = buflen;

#ifdef DEBUG
    /* Debugging code. In theory useless, because it's debugged ;-) */
    printf("IE raw value %d [%02X", buflen, iebuf[0]);
    for(i = 1; i < buflen; i++)
        printf(":%02X", iebuf[i]);
    printf("]\n");
#endif

    switch(iebuf[0])
    {
        case 0x30:		/* WPA2 */
            /* Check if we have enough data */
            if(ielen < 4)
            {
                iw_print_ie_unknown(iebuf, buflen);
                return;
            }

            wpa_oui = wpa2_oui;
            break;

        case 0xdd:		/* WPA or else */
            wpa_oui = wpa1_oui;

            /* Not all IEs that start with 0xdd are WPA. 
            * So check that the OUI is valid. Note : offset==2 */
            if((ielen < 8) || (memcmp(&iebuf[offset], wpa_oui, 3) != 0) || (iebuf[offset + 3] != 0x01))
            {
                iw_print_ie_unknown(iebuf, buflen);
                return;
            }

            /* Skip the OUI type */
            offset += 4;
            break;

        default:
            return;
    }

    /* Pick version number (little endian) */
    ver = iebuf[offset] | (iebuf[offset + 1] << 8);
    offset += 2;

    if(iebuf[0] == 0xdd)
        printf("WPA Version %d\n", ver);
    if(iebuf[0] == 0x30)
        printf("IEEE 802.11i/WPA2 Version %d\n", ver);

    /* From here, everything is technically optional. */
    /* Check if we are done */
    if(ielen < (offset + 4))
    {
        /* We have a short IE.  So we should assume TKIP/TKIP. */
        printf("                        Group Cipher : TKIP\n");
        printf("                        Pairwise Cipher : TKIP\n");
        return;
    }

    /* Next we have our group cipher. */
    if(memcmp(&iebuf[offset], wpa_oui, 3) != 0)
    {
        printf("                        Group Cipher : Proprietary\n");
    }
    else
    {
        printf("                        Group Cipher :");
        iw_print_value_name(iebuf[offset+3],iw_ie_cypher_name, IW_IE_CYPHER_NUM);

        printf("\n");
    }
    offset += 4;

    /* Check if we are done */
    if(ielen < (offset + 2))
    {
        /* We don't have a pairwise cipher, or auth method. Assume TKIP. */
        printf("                        Pairwise Ciphers : TKIP\n");
        return;
    }

    /* Otherwise, we have some number of pairwise ciphers. */
    cnt = iebuf[offset] | (iebuf[offset + 1] << 8);
    offset += 2;
    printf("                        Pairwise Ciphers (%d) :", cnt);

    if(ielen < (offset + 4*cnt))
        return;

    for(i = 0; i < cnt; i++)
    {
        if(memcmp(&iebuf[offset], wpa_oui, 3) != 0)
        {
            printf(" Proprietary");
        }
        else
        {
            iw_print_value_name(iebuf[offset+3], iw_ie_cypher_name, IW_IE_CYPHER_NUM);
            if(iebuf[0] == 0xdd) //WPA
            {
                if (iebuf[offset+3] == 2) //WPA_TKIP
                {
                    if (router_list->encryptType == ENCRYPT_WPA2_TKIP)
                    {
                        router_list->encryptType = ENCRYPT_WPA1_WPA2_TKIP;
                    }
                    else if (router_list->encryptType == ENCRYPT_WPA2_AES)
                    {
                        router_list->encryptType = ENCRYPT_WPA1_TKIP_WPA2_AES;
                    }
                    else
                    {
                        router_list->encryptType = ENCRYPT_WPA1_TKIP;
                    }
                }
                else if (iebuf[offset+3] == 4) //WPA_AES
                {
                    if (router_list->encryptType == ENCRYPT_WPA2_AES)
                    {
                        router_list->encryptType = ENCRYPT_WPA1_WPA2_AES;
                    }
                    else if (router_list->encryptType == ENCRYPT_WPA2_TKIP)
                    {
                        router_list->encryptType = ENCRYPT_WPA1_AES_WPA2_TKIP;
                    }
                    else
                    {
                        router_list->encryptType = ENCRYPT_WPA1_AES;
                    }
                }
                else
                {
                    router_list->encryptType = ENCRYPT_NULL;
                }
            }
            else if(iebuf[0] == 0x30) //WPA2
            {
                if (iebuf[offset+3] == 2) //WPA_TKIP
                {
                    if (router_list->encryptType == ENCRYPT_WPA1_TKIP)
                    {
                        router_list->encryptType = ENCRYPT_WPA1_WPA2_TKIP;
                    }
                    else if (router_list->encryptType == ENCRYPT_WPA1_AES)
                    {
                        router_list->encryptType = ENCRYPT_WPA1_AES_WPA2_TKIP;
                    }
                    else
                    {
                        router_list->encryptType = ENCRYPT_WPA2_TKIP;
                    }
                }
                else if (iebuf[offset+3] == 4) //WPA_AES
                {
                    if (router_list->encryptType == ENCRYPT_WPA1_AES)
                    {
                        router_list->encryptType = ENCRYPT_WPA1_WPA2_AES;
                    }
                    else if (router_list->encryptType == ENCRYPT_WPA1_TKIP)
                    {
                        router_list->encryptType = ENCRYPT_WPA1_TKIP_WPA2_AES;
                    }
                    else
                    {
                        router_list->encryptType = ENCRYPT_WPA2_AES;
                    }
                }
                else
                {
                    router_list->encryptType = ENCRYPT_NULL;
                }
            }
            else
            {
                router_list->encryptType = ENCRYPT_NULL;
            }
        }
        offset+=4;
    }
    printf("\n");

    /* Check if we are done */
    if(ielen < (offset + 2))
        return;

    /* Now, we have authentication suites. */
    cnt = iebuf[offset] | (iebuf[offset + 1] << 8);
    offset += 2;
    printf("                        Authentication Suites (%d) :", cnt);

    if(ielen < (offset + 4*cnt))
        return;

    for(i = 0; i < cnt; i++)
    {
        if(memcmp(&iebuf[offset], wpa_oui, 3) != 0)
        {
            printf(" Proprietary");
        }
        else
        {
            iw_print_value_name(iebuf[offset+3], iw_ie_key_mgmt_name, IW_IE_KEY_MGMT_NUM);
        }
        offset+=4;
    }
    printf("\n");

    /* Check if we are done */
    if(ielen < (offset + 1))
        return;

    /* Otherwise, we have capabilities bytes.
    * For now, we only care about preauth which is in bit position 1 of the
    * first byte.  (But, preauth with WPA version 1 isn't supposed to be 
    * allowed.) 8-) */
    if(iebuf[offset] & 0x01)
    {
        printf("                       Preauthentication Supported\n");
    }
}
 
/*------------------------------------------------------------------*/
/*
 * Process a generic IE and display the info in human readable form
 * for some of the most interesting ones.
 * For now, we only decode the WPA IEs.
 */
static inline void iw_print_gen_ie(unsigned char *	buffer, int	buflen, router_list_s *router_list)
{
    int offset = 0;

    /* Loop on each IE, each IE is minimum 2 bytes */
    while(offset <= (buflen - 2))
    {
        printf("                    IE: ");

        /* Check IE type */
        switch(buffer[offset])
        {
            case 0xdd:	/* WPA1 (and other) */
            case 0x30:	/* WPA2 */
                iw_print_ie_wpa(buffer + offset, buflen, router_list);
                break;
            default:
                iw_print_ie_unknown(buffer + offset, buflen);
        }
        /* Skip over this IE to the next one in the list. */
        offset += buffer[offset+1] + 2;
    }
}
#endif	/* WE_ESSENTIAL */

/***************************** SCANNING *****************************/
/*
 * This one behave quite differently from the others
 *
 * Note that we don't use the scanning capability of iwlib (functions
 * iw_process_scan() and iw_scan()). The main reason is that
 * iw_process_scan() return only a subset of the scan data to the caller,
 * for example custom elements and bitrates are ommited. Here, we
 * do the complete job...
 */

/*------------------------------------------------------------------*/
/*
 * Print one element from the scanning results
 */
static inline void
print_scanning_token(struct stream_descr *	stream,	/* Stream of events */
		     struct iw_event *		event,	/* Extracted token */
		     struct iwscan_state *	state,
		     struct iw_range *	iw_range,	/* Range info */
		     int		has_range,
		     router_list_s **router_list, 
		     int *router_num)
{
    char		buffer[128];	/* Temporary buffer */
    
    /* Now, let's decode the event */
    switch(event->cmd)
    {
        case SIOCGIWAP:
        {
            printf("          Cell %02d - Address: %s\n", state->ap_num,iw_saether_ntop(&event->u.ap_addr, buffer));
            *router_num = state->ap_num;
            state->ap_num++;
            break;
        }
        case SIOCGIWNWID:
        {
            #if 0
            if(event->u.nwid.disabled)
                printf("                    NWID:off/any\n");
            else
                printf("                    NWID:%X\n", event->u.nwid.value);
            #endif
            break;
        }
        case SIOCGIWFREQ:
        {
            double		freq;			/* Frequency/channel */
            int		channel = -1;		/* Converted to channel */
            freq = iw_freq2float(&(event->u.freq));
            /* Convert to channel if possible */
            if(has_range)
                channel = iw_freq_to_channel(freq, iw_range);
            iw_print_freq(buffer, sizeof(buffer),freq, channel, event->u.freq.flags);
            printf("                    %s\n", buffer);
        
            break;
        }
        case SIOCGIWMODE:
            /* Note : event->u.mode is unsigned, no need to check <= 0 */
            if(event->u.mode >= IW_NUM_OPER_MODE)event->u.mode = IW_NUM_OPER_MODE;
            {
                printf("                    Mode:%s\n", iw_operation_mode[event->u.mode]);
                ((*router_list) + state->ap_num - 2)->mode = event->u.mode;
            }
            break;
        case SIOCGIWNAME:
            printf("                    Protocol:%-1.16s\n", event->u.name);
            break;
        case SIOCGIWESSID:
            {
                char essid[IW_ESSID_MAX_SIZE+1];
                memset(essid, '\0', sizeof(essid));
                memset(((*router_list) + state->ap_num - 2)->essid, '\0', sizeof((*router_list)->essid));
                
                if((event->u.essid.pointer) && (event->u.essid.length))
                {
                    memcpy(essid, event->u.essid.pointer, event->u.essid.length);
                    memcpy(((*router_list) + state->ap_num - 2)->essid, event->u.essid.pointer, event->u.essid.length);
                }
                if(event->u.essid.flags)
                {
                    /* Does it have an ESSID index ? */
                    if((event->u.essid.flags & IW_ENCODE_INDEX) > 1)
                        printf("                    ESSID:\"%s\" [%d]\n", essid,(event->u.essid.flags & IW_ENCODE_INDEX));
                    else
                        printf("                    ESSID:\"%s\"\n", essid);
                }
                else
                    printf("                    ESSID:off/any/hidden\n");
            }
            break;
        case SIOCGIWENCODE:
            {
                unsigned char	key[IW_ENCODING_TOKEN_MAX];
                
                if(event->u.data.pointer)
                {
                    memcpy(key, event->u.data.pointer, event->u.data.length);
                }
                else
                    event->u.data.flags |= IW_ENCODE_NOKEY;
                printf("                    Encryption key:");
                if(event->u.data.flags & IW_ENCODE_DISABLED)
                {
                    printf("off\n");
                    ((*router_list) + state->ap_num - 2)->encryptType = ENCRYPT_NONE;
                }
                else
                {
                    //((*router_list) + state->ap_num - 2)->encryptType = ENCRYPT_WEP;
                    /* Display the key */
                    iw_print_key(buffer, sizeof(buffer), key, event->u.data.length, event->u.data.flags);
                    printf("%s", buffer);

                    printf("event->u.data.flags = %x\n", event->u.data.flags);
                    /* Other info... */
                    if((event->u.data.flags & IW_ENCODE_INDEX) > 1)
                        printf(" [%d]", event->u.data.flags & IW_ENCODE_INDEX);
                    if(event->u.data.flags & IW_ENCODE_RESTRICTED)
                        printf("   Security mode:restricted");
                    if(event->u.data.flags & IW_ENCODE_OPEN)
                        printf("   Security mode:open");
                    printf("\n");
                }
            }
            break;
        case SIOCGIWRATE:
            if(state->val_index == 0)
                printf("                    Bit Rates:");
            else
                if((state->val_index % 5) == 0)
                    printf("\n                              ");
                else
                    printf("; ");
            iw_print_bitrate(buffer, sizeof(buffer), event->u.bitrate.value);
            printf("%s", buffer);
            /* Check for termination */
            if(stream->value == NULL)
            {
                printf("\n");
                state->val_index = 0;
            }
            else
                state->val_index++;
            break;
        case SIOCGIWMODUL:
            {
                unsigned int	modul = event->u.param.value;
                int		i;
                int		n = 0;
                printf("                    Modulations :");
                for(i = 0; i < IW_SIZE_MODUL_LIST; i++)
                {
                    if((modul & iw_modul_list[i].mask) == iw_modul_list[i].mask)
                    {
                        if((n++ % 8) == 7)
                            printf("\n                        ");
                        else
                            printf(" ; ");
                        printf("%s", iw_modul_list[i].cmd);
                    }
                }
                printf("\n");
            }
            break;
        case IWEVQUAL:
            iw_print_stats(buffer, sizeof(buffer),&event->u.qual, iw_range, has_range);
            ((*router_list) + state->ap_num - 2)->quality = ((float)event->u.qual.qual/(float)iw_range->max_qual.qual) * 100;
            printf("                    %s\n", buffer);
            break;
#ifndef WE_ESSENTIAL
        case IWEVGENIE:
            /* Informations Elements are complex, let's do only some of them */
            iw_print_gen_ie((unsigned char *)event->u.data.pointer, event->u.data.length, (*router_list) + state->ap_num - 2);
            break;
#endif	/* WE_ESSENTIAL */
        case IWEVCUSTOM:
            {
                char custom[IW_CUSTOM_MAX+1];
                if((event->u.data.pointer) && (event->u.data.length))
                memcpy(custom, event->u.data.pointer, event->u.data.length);
                custom[event->u.data.length] = '\0';
                printf("                    Extra:%s\n", custom);
            }
            break;
        default:
            printf("                    (Unknown Wireless Token 0x%04X)\n",event->cmd);
    }	/* switch(event->cmd) */
}

/*------------------------------------------------------------------*/
/*
 * Perform a scanning on one device
 */
static int print_scanning_info(int skfd, char *	ifname, char *args[], int count, router_list_s **router_list, int *router_num)
{
    struct iwreq		wrq;
    int			scanflags = 0;		/* Flags for scan */
    unsigned char *	buffer = NULL;		/* Results */
    int			buflen = IW_SCAN_MAX_DATA; /* Min for compat WE<17 */
    struct iw_range	range;
    int			has_range;
    struct timeval	tv;				/* Select timeout */
    int			timeout = 15000000;		/* 15s */

    /* Get range stuff */
    has_range = (iw_get_range_info(skfd, ifname, &range) >= 0);

    /* Check if the interface could support scanning. */
    if((!has_range) || (range.we_version_compiled < 14))
    {
        fprintf(stderr, "%-8.16s  Interface doesn't support scanning.\n\n", ifname);
        return(-1);
    }

    /* Init timeout value -> 250ms between set and first get */
    tv.tv_sec = 0;
    tv.tv_usec = 250000;
    
    wrq.u.data.pointer = NULL;
    wrq.u.data.flags = 0;
    wrq.u.data.length = 0;


    /* Initiate Scanning */
    if(iw_set_ext(skfd, ifname, SIOCSIWSCAN, &wrq) < 0)
    {
        if(errno != EPERM)
        {
            fprintf(stderr, "%-8.16s  Interface doesn't support scanning : %s\n\n",ifname, strerror(errno));
            return(-1);
        }
        /* If we don't have the permission to initiate the scan, we may
        * still have permission to read left-over results.
        * But, don't wait !!! */

        tv.tv_usec = 0;
    }

    timeout -= tv.tv_usec;

    /* Forever */
    while(1)
    {
        fd_set		rfds;		/* File descriptors for select */
        int		last_fd;	/* Last fd */
        int		ret;

        /* Guess what ? We must re-generate rfds each time */
        FD_ZERO(&rfds);
        last_fd = -1;

        /* In here, add the rtnetlink fd in the list */

        /* Wait until something happens */
        ret = select(last_fd + 1, &rfds, NULL, NULL, &tv);

        /* Check if there was an error */
        if(ret < 0)
        {
            if(errno == EAGAIN || errno == EINTR)
                continue;
            fprintf(stderr, "Unhandled signal - exiting...\n");
            return(-1);
        }

        /* Check if there was a timeout */
        if(ret == 0)
        {
            unsigned char *	newbuf;

            realloc:
            /* (Re)allocate the buffer - realloc(NULL, len) == malloc(len) */
            newbuf = (unsigned char *)realloc(buffer, buflen);
            if(newbuf == NULL)
            {
                if(buffer)
                    free(buffer);
                fprintf(stderr, "%s: Allocation failed\n", __FUNCTION__);
                return(-1);
            }
            buffer = newbuf;
            /* Try to read the results */
            wrq.u.data.pointer = buffer;
            wrq.u.data.flags = 0;
            wrq.u.data.length = buflen;
            if(iw_get_ext(skfd, ifname, SIOCGIWSCAN, &wrq) < 0)
            {
                /* Check if buffer was too small (WE-17 only) */
                if((errno == E2BIG) && (range.we_version_compiled > 16))
                {
                    /* Some driver may return very large scan results, either
                    * because there are many cells, or because they have many
                    * large elements in cells (like IWEVCUSTOM). Most will
                    * only need the regular sized buffer. We now use a dynamic
                    * allocation of the buffer to satisfy everybody. Of course,
                    * as we don't know in advance the size of the array, we try
                    * various increasing sizes. Jean II */

                    /* Check if the driver gave us any hints. */
                    if(wrq.u.data.length > buflen)
                        buflen = wrq.u.data.length;
                    else
                        buflen *= 2;

                    /* Try again */
                    goto realloc;
                }

                /* Check if results not available yet */
                if(errno == EAGAIN)
                {
                    /* Restart timer for only 100ms*/
                    tv.tv_sec = 0;
                    tv.tv_usec = 100000;
                    timeout -= tv.tv_usec;
                    if(timeout > 0)
                        continue;	/* Try again later */
                }

                /* Bad error */
                free(buffer);
                fprintf(stderr, "%-8.16s  Failed to read scan data : %s\n\n",
                ifname, strerror(errno));
                return(-2);
            }
            else
                /* We have the results, go to process them */
                break;
        }

    /* In here, check if event and event type
    * if scan event, read results. All errors bad & no reset timeout */
    }

    if(wrq.u.data.length)
    {
        struct iw_event		iwe;
        struct stream_descr	stream;
        struct iwscan_state	state = { /*.ap_num = */1, /*.val_index = */0 };
        int			ret;

        printf("%-8.16s  Scan completed :\n", ifname);
        iw_init_event_stream(&stream, (char *) buffer, wrq.u.data.length);
        do
        {
            /* Extract an event and print it */
            ret = iw_extract_event_stream(&stream, &iwe, range.we_version_compiled);
            if(ret > 0)
                print_scanning_token(&stream, &iwe, &state,&range, has_range, router_list, router_num);
        }
        while(ret > 0 && *router_num < 20);
        printf("\n");
    }
    else
    {
        printf("%-8.16s  No scan results\n\n", ifname);
        *router_num = 0;
    }
    free(buffer);
    return(0);
}

int get_wrieless_router_list(router_list_s **router_list, int *router_num, char *wifi_name)
{
    int skfd;

    if((skfd = iw_sockets_open()) < 0)
    {
        perror("socket");
        return -1;
    }

    print_scanning_info(skfd, wifi_name, NULL, 0, router_list, router_num);

    iw_sockets_close(skfd);

    return 0;
}
