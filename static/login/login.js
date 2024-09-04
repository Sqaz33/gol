function login() {
    const data = {
        login: document.getElementById("login").value,
        pword: document.getElementById("pword").value
    };

    fetch('/login', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(data)
    })
    .then(response => response.text())
    .then(result => {
        window.alert(result);
    })
    .catch(error => {
        console.error('Ошибка:', error);
    });

}