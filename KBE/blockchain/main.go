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
