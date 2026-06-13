- Must be able to handle concurrent transfers
- Must accept:
    - --blksize [int]
    - --timout [secs]
    - --max-retries [int]
- retransmit if ACK isn't received
- send appropriate error codes
- basically, just do everything the RFC details

---

When a read request is received, we need to:
- See if the file exists
- Get the length of the file
- Read the file in chunks (according to blksize)
    - We don't want to bring the whole file into memory
- Send each chunk, with the packet number, to requestor
- Wait for ack
    - if error, stop
- Get next chunk, send, and repeat till done/error

Store this struct in a map with IP address as key
```c
struct transfer_info {
        filename,
        blocksize,
        current_block_index,
        final_block_index
}
```
