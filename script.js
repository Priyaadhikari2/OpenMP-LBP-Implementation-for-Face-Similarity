document.getElementById("lbpForm").addEventListener("submit", async (e) => {
  e.preventDefault();

  const fileInput = document.getElementById("imageInput");
  const mode = document.getElementById("mode").value;
  const result = document.getElementById("result");

  if (!fileInput.files.length) {
    alert("Please upload an image file first!");
    return;
  }

  const formData = new FormData();
  formData.append("file", fileInput.files[0]);
  formData.append("mode", mode);

  result.textContent = "Processing... please wait ⏳";

  try {
    const res = await fetch("http://localhost:5000/run-lbp", {
      method: "POST",
      body: formData
    });

    const data = await res.json();
    result.textContent = data.output;
  } catch (err) {
    result.textContent = "❌ Error: " + err.message;
  }
});
