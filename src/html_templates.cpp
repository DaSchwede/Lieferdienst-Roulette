#include <Arduino.h>
#include "html_templates.h"
#include "storage.h"
#include "rtc.h"

String getMainPageHTML() {
  String gespeicherteDienste = loadLieferdienste();
  if (gespeicherteDienste == "") gespeicherteDienste = "[]";

  String wochentag = getWochentagName();
  String datum = getDatum();
  String zeit = getZeit();

  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta charset="UTF-8">
    <title>Lieferdienst-Verwaltung</title>
    <style>
      body {
        font-family: sans-serif;
        background: #f0f0f0;
        display: flex;
        justify-content: center;
        padding: 30px;
      }
      .container {
        background: #fff;
        padding: 20px 30px;
        border-radius: 10px;
        box-shadow: 0 0 10px rgba(0,0,0,0.1);
        max-width: 600px;
        width: 100%;
      }
      h1 {
        text-align: center;
        color: #333;
      }
      input[type="text"] {
        width: 100%;
        padding: 8px;
        margin: 8px 0;
        box-sizing: border-box;
      }
      button {
        padding: 6px 12px;
        margin: 4px;
        border: none;
        border-radius: 5px;
        cursor: pointer;
      }
      button:hover {
        opacity: 0.9;
      }
      .add-btn {
        background: #4CAF50;
        color: white;
      }
      .edit-btn {
        background: #ff9800;
        color: white;
      }
      .delete-btn {
        background: #f44336;
        color: white;
      }
      table {
        width: 100%;
        border-collapse: collapse;
        margin-top: 20px;
      }
      th, td {
        border: 1px solid #ccc;
        padding: 8px;
        text-align: left;
      }
      th {
        background: #eee;
      }
      label {
        margin-right: 10px;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>Lieferdienst-Verwaltung</h1>

    <p style='text-align:center;'>Heute ist <b>)rawliteral" + wochentag + R"rawliteral(</b>, der <span id='datumAnzeige'>)rawliteral" + datum + R"rawliteral(</span>, <span id='zeitAnzeige'>)rawliteral" + zeit + R"rawliteral(</span>
  <button onclick='zeigeZeitEditor()' style='border:none;background:none;cursor:pointer;'>✎</button></p>
  <div id='zeitEditor' style='display:none;text-align:center;'>
    <input type='datetime-local' id='neueZeit'>
    <button onclick='zeitSpeichern()'>💾 Uhrzeit setzen</button>
  </div>

      <h3>Neuen Lieferdienst hinzufügen</h3>
      <input type="text" id="dienstName" placeholder="z. B. Lieferando">

      <div>
        <label><input type="checkbox" value="Montag"> Mo</label>
        <label><input type="checkbox" value="Dienstag"> Di</label>
        <label><input type="checkbox" value="Mittwoch"> Mi</label>
        <label><input type="checkbox" value="Donnerstag"> Do</label>
        <label><input type="checkbox" value="Freitag"> Fr</label>
        <label><input type="checkbox" value="Samstag"> Sa</label>
        <label><input type="checkbox" value="Sonntag"> So</label>
      </div>

      <button class="add-btn" onclick="hinzufuegen()">➕ Hinzufügen</button>

      <h3>Gespeicherte Lieferdienste</h3>
      <div id="dienstListe"></div>
    </div>

    <script>
      let dienste = JSON.parse(`)rawliteral" + gespeicherteDienste + R"rawliteral(`);
      let editIndex = -1;

      function zeigeZeitEditor() {
  document.getElementById('zeitEditor').style.display = 'block';
}

let aktuelleZeit = new Date();
  let [hh, mm, ss] = ")rawliteral" + zeit + R"rawliteral(".split(":").map(Number);
  let [dd, MM, yyyy] = ")rawliteral" + datum + R"rawliteral(".split(".").map(Number);

  aktuelleZeit.setFullYear(yyyy);
  aktuelleZeit.setMonth(MM - 1); // JavaScript: 0 = Januar
  aktuelleZeit.setDate(dd);
  aktuelleZeit.setHours(hh);
  aktuelleZeit.setMinutes(mm);
  aktuelleZeit.setSeconds(ss);

  function updateZeit() {
    aktuelleZeit.setSeconds(aktuelleZeit.getSeconds() + 1);

    const h = String(aktuelleZeit.getHours()).padStart(2, '0');
    const m = String(aktuelleZeit.getMinutes()).padStart(2, '0');
    const s = String(aktuelleZeit.getSeconds()).padStart(2, '0');
    document.getElementById('zeitAnzeige').textContent = h + ":" + m + ":" + s;

    const heute = aktuelleZeit.getDate() + "." +
                  String(aktuelleZeit.getMonth() + 1).padStart(2, '0') + "." +
                  aktuelleZeit.getFullYear();
    document.getElementById('datumAnzeige').textContent = heute;
  }

  setInterval(updateZeit, 1000);

function zeitSpeichern() {
  const input = document.getElementById('neueZeit').value;
  if (!input) return alert("Bitte Datum und Uhrzeit wählen.");
  fetch("/set_time", {
    method: "POST",
    headers: { "Content-Type": "application/x-www-form-urlencoded" },
    body: "datetime=" + encodeURIComponent(input)
  })
  .then(res => res.ok ? location.reload() : alert("Fehler beim Setzen der Zeit."));
}


      function renderListe() {
        const div = document.getElementById('dienstListe');
        if (dienste.length === 0) {
          div.innerHTML = "<p>Keine Einträge vorhanden.</p>";
          return;
        }

        let html = "<table><tr><th>Name</th><th>Ruhetage</th><th>Aktionen</th></tr>";
        dienste.forEach((d, index) => {
          if (editIndex === index) {
            html += `
              <tr>
                <td><input type="text" id="editName${index}" value="${d.name}"></td>
                <td>
                  ${["Montag","Dienstag","Mittwoch","Donnerstag","Freitag","Samstag","Sonntag"].map(tag => `
                    <label><input type="checkbox" value="${tag}" ${d.ruhe.includes(tag) ? 'checked' : ''} class="editRuhe${index}"> ${tag.slice(0,2)}</label>
                  `).join(' ')}
                </td>
                <td>
                  <button class="edit-btn" onclick="speichernEdit(${index})">💾</button>
                  <button class="delete-btn" onclick="abbrechen()">❌</button>
                </td>
              </tr>
            `;
          } else {
            const ruhe = d.ruhe.length ? d.ruhe.join(', ') : '–';
            html += `
              <tr>
                <td>${d.name}</td>
                <td>${ruhe}</td>
                <td>
                  <button class="edit-btn" onclick="editieren(${index})">📝</button>
                  <button class="delete-btn" onclick="loeschen(${index})">🗑️</button>
                </td>
              </tr>
            `;
          }
        });
        html += "</table>";
        div.innerHTML = html;
      }

      function editieren(index) {
        editIndex = index;
        renderListe();
      }

      function abbrechen() {
        editIndex = -1;
        renderListe();
      }

      function speichernEdit(index) {
        const name = document.getElementById('editName' + index).value.trim();
        const ruhetage = [...document.querySelectorAll('.editRuhe' + index + ':checked')].map(cb => cb.value);
        if (!name) return alert("Name darf nicht leer sein");

        dienste[index] = { name, ruhe: ruhetage };
        editIndex = -1;
        speichern();
      }

      function hinzufuegen() {
        const name = document.getElementById('dienstName').value.trim();
        if (!name) return alert("Bitte Name eingeben");
        const ruhetage = [...document.querySelectorAll('input[type=checkbox]:checked')].map(cb => cb.value);
        dienste.push({ name, ruhe: ruhetage });
        speichern();
      }

      function loeschen(index) {
        if (confirm("Eintrag löschen?")) {
          dienste.splice(index, 1);
          speichern();
        }
      }

      function speichern() {
        fetch('/save_json', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(dienste)
        })
        .then(res => {
          if (res.ok) {
            renderListe();
            document.getElementById('dienstName').value = '';
            document.querySelectorAll('input[type=checkbox]').forEach(cb => cb.checked = false);
          } else {
            alert("Fehler beim Speichern.");
          }
        })
        .catch(err => {
          console.error("Fehler:", err);
          alert("Speicherfehler");
        });
      }

      renderListe();
    </script>
  </body>
  </html>
  )rawliteral";

  return html;
}
