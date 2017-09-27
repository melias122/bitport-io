package auth

import (
	"bytes"
	"encoding/json"
	"fmt"
	"net/http"
	"net/url"
	"time"

	"github.com/melias122/bitport/bitport"
	"golang.org/x/oauth2"
)

type AccessCode struct {
	ClientID     string
	ClientSecret string
	ClientCode   string
}

func (u *AccessCode) Token() (*oauth2.Token, error) {

	v := url.Values{}
	v.Add("client_id", u.ClientID)
	v.Add("client_secret", u.ClientSecret)
	v.Add("code", u.ClientCode)
	v.Add("grant_type", "code")

	body := bytes.NewBufferString(v.Encode())

	req, err := http.NewRequest("POST", bitport.Endpoint.TokenURL, body)
	if err != nil {
		return nil, err
	}
	req.Header.Add("Content-Type", "application/x-www-form-urlencoded")

	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	if resp.StatusCode != 200 {
		return nil, fmt.Errorf("unable to authorize")
	}

	var tok tokenJSON
	if err := json.NewDecoder(resp.Body).Decode(&tok); err != nil {
		return nil, err
	}
	otok := &oauth2.Token{
		AccessToken:  tok.AccessToken,
		TokenType:    tok.TokenType,
		RefreshToken: tok.RefreshToken,
		Expiry:       tok.expiry(),
	}

	if !otok.Valid() {
		return nil, fmt.Errorf("ivalid token")
	}

	return otok, nil
}

// tokenJSON is the struct representing the HTTP response from OAuth2
// providers returning a token in JSON form.
type tokenJSON struct {
	AccessToken  string         `json:"access_token"`
	TokenType    string         `json:"token_type"`
	RefreshToken string         `json:"refresh_token"`
	ExpiresIn    expirationTime `json:"expires_in"`
}

func (e *tokenJSON) expiry() (t time.Time) {
	if v := e.ExpiresIn; v != 0 {
		return time.Now().Add(time.Duration(v) * time.Second)
	}
	return
}

type expirationTime int32

func (e *expirationTime) UnmarshalJSON(b []byte) error {
	var n json.Number
	err := json.Unmarshal(b, &n)
	if err != nil {
		return err
	}
	i, err := n.Int64()
	if err != nil {
		return err
	}
	*e = expirationTime(i)
	return nil
}
