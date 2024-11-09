from flask import Flask, render_template, request, redirect, url_for, abort, send_from_directory, Response
from flask_sqlalchemy import SQLAlchemy
from PIL import Image
from werkzeug.utils import secure_filename
from datetime import datetime
import uuid
import os
import logging
logging.basicConfig(level=logging.DEBUG)

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///posts.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config['UPLOAD_FOLDER'] = 'static/uploads/'
app.config['MAX_CONTENT_LENGTH'] = 3 * 1024 * 1024  

app.secret_key = os.getenv('SECRET_KEY', 'test_key')

db = SQLAlchemy(app)


class Post(db.Model):
    id = db.Column(db.String(36), primary_key=True, unique=True, nullable=False)
    title = db.Column(db.String(100), nullable=False)
    content = db.Column(db.Text, nullable=False)
    image_filename = db.Column(db.String(100), nullable=True)
    script_pack = db.Column(db.String(100), nullable=True)
    date_posted = db.Column(db.DateTime, nullable=False, default=datetime.utcnow)

def initialize_posts():
    if Post.query.count() == 0: 
        initial_posts = [
            Post(
                id=str(uuid.uuid4()), 
                title="Exploring the Alps", 
                content="Discover the majestic landscapes and vibrant culture of the Alpine regions...", 
                image_filename="alps.webp", 
                script_pack=None,
                date_posted=datetime(2024, 11, 9) 
            ),
            Post(
                id=str(uuid.uuid4()), 
                title="A Weekend in Tokyo", 
                content="Immerse yourself in the bustling streets, incredible food, and peaceful temples of Tokyo...", 
                image_filename="tokyo.webp", 
                script_pack=None,
                date_posted=datetime(2024, 11, 9) 
            ),
            Post(
                id=str(uuid.uuid4()), 
                title="Safari Adventure", 
                content="Experience the thrill of the wild on an unforgettable safari journey...", 
                image_filename="safari.webp", 
                script_pack="js/nightPack.js",
                date_posted=datetime(2024, 11, 9) 
            ),
        ]
        db.session.bulk_save_objects(initial_posts)
        db.session.commit()

with app.app_context():
    db.create_all()
    initialize_posts()

@app.route('/')
def home():
    posts = Post.query.limit(3).all() 
    return render_template('main.html', posts=posts)

@app.route('/about')
def about():
    return render_template('about.html')

@app.route('/entry/<post_id>')
def entry(post_id):
    post = Post.query.filter_by(id=post_id).first()
    if post is None:
        abort(404)
    return render_template('post.html', post=post)


def is_webp(file):
    try:
        with Image.open(file.stream) as img:
            if img.format != 'WEBP':
                return False
            # check for corruption
            img.load()
            file.stream.seek(0)  
            return True
    except Exception as e:
        return False


@app.route('/upload_blog_post', methods=['GET', 'POST'])
def upload_post():
    if request.method == 'POST':
        post_title = request.form.get('post_title')
        post_content = request.form.get('post_body')
        script_pack = request.form.get('script_pack') 


        if 'file' not in request.files:
            return "No file part in the request", 400
        file = request.files['file']

        if file.filename == '':
            return "No selected file", 400

        if file and is_webp(file):
            try:
                extension = file.filename.rsplit('.', 1)[1].lower()
                filename = f"{uuid.uuid4()}.{extension}"
                filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
                file.save(filepath)

                new_post = Post(
                    id=str(uuid.uuid4()), 
                    title=post_title, 
                    content=post_content, 
                    image_filename=filename, 
                    date_posted=datetime.utcnow()
                )
                if script_pack:
                    new_post.script_pack = script_pack

                db.session.add(new_post)
                db.session.commit()

                return redirect(url_for('entry', post_id=new_post.id))
            except Exception as e:
                logging.error(f"Error saving post: {e}")
                abort(500)
        else:
            return "Invalid file type. Only WEBP images are allowed.", 400

    return render_template('upload.html')

@app.route('/polyglot_admirals_club')
def polyglot_admirals_club():
    if request.cookies.get('exec_cookie') == os.environ.get('exec_cookie', 'TESTING_TESTING'):
        flag = os.environ.get('FLAG', 'flag{TESTING_TESTING}')
        return render_template('polyglot_admirals_club.html', flag=flag)
    else:
        abort(403) 

if __name__ == '__main__':
    app.run(debug=True)
