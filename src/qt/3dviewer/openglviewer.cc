#include "openglviewer.h"

OpenGLViewer::OpenGLViewer(QWidget *parent)
    : QOpenGLWidget{parent},
      x_rotate{0},
      y_rotate{0},
      vertices{nullptr},
      faces{nullptr},
      vertex_count{0},
      face_count{0},
      tx{0},
      ty{0},
      tz{0},
      x_slider{nullptr},
      y_slider{nullptr},
      z_slider{nullptr},
      camera_x{0.0f},
      camera_y{0.0f},
      camera_z{-10.0f},
      current_tx{0.0f},
      current_ty{0.0f},
      current_tz{0.0f},
      total_angle_x{0.0f},
      total_angle_y{0.0f},
      total_angle_z{0.0f},
      current_scale{1.0f},
      ortho_scale{10.0f},
      perspective_view{true},
      background_color{Qt::black},
      edge_color{Qt::green},
      vertex_color{Qt::green},
      edge_thickness{1.0f},
      edge_dashed{false},
      vertex_size{0.0f},
      vertex_shape{"None"} {
  setFocusPolicy(Qt::StrongFocus);
}

OpenGLViewer::~OpenGLViewer() { clear(); }

void OpenGLViewer::initializeGL() {
  QOpenGLFunctions *func = QOpenGLContext::currentContext()->functions();
  initializeOpenGLFunctions();
  func->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  func->glEnable(GL_DEPTH_TEST);
  GLint samples{0};
  glGetIntegerv(GL_SAMPLES, &samples);
  glShadeModel(GL_FLAT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLViewer::resizeGL(GLint width, GLint height) {
  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
}

void OpenGLViewer::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  update_background_color();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (perspective_view) {
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 50.0);
  } else {
    glOrtho(-ortho_scale, ortho_scale, -ortho_scale, ortho_scale, -100.0,
            100.0);
  }
  glTranslatef(camera_x, camera_y, camera_z);
  glRotatef(x_rotate + 30, 1.0, 0.0, 0.0);
  glRotatef(y_rotate + 45, 0.0, 1.0, 0.0);
  glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
  glPushAttrib(GL_LINE_BIT);
  glLineWidth(1.0f);
  draw_grid();
  glPopAttrib();
  glColor3f(edge_color.redF(), edge_color.greenF(), edge_color.blueF());
  glLineWidth(edge_thickness);
  if (edge_dashed) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  reset_rotation();
  if (faces && vertices) {
    create_model();
  } else {
    create_default_cube();
  }
  glDisable(GL_LINE_STIPPLE);
}

void OpenGLViewer::mousePressEvent(QMouseEvent *pe) { position = pe->pos(); }

void OpenGLViewer::mouseMoveEvent(QMouseEvent *pe) {
  x_rotate += 180 * (GLfloat)(pe->position().y() - position.y()) / height();
  y_rotate += 180 * (GLfloat)(pe->position().x() - position.x()) / width();
  update();
  position = pe->pos();
}

void OpenGLViewer::keyPressEvent(QKeyEvent *event) {
  const float move_speed = 0.3f;
  const float scale_speed = 0.3f;
  if (perspective_view) {
    switch (event->key()) {
      case Qt::Key_W:
        camera_z += move_speed;
        break;
      case Qt::Key_S:
        camera_z -= move_speed;
        break;
      case Qt::Key_A:
        camera_x += move_speed;
        break;
      case Qt::Key_D:
        camera_x -= move_speed;
        break;
      case Qt::Key_Q:
        camera_y -= move_speed;
        break;
      case Qt::Key_E:
        camera_y += move_speed;
        break;
    }
  } else {
    switch (event->key()) {
      case Qt::Key_W:
        ortho_scale -= scale_speed;
        if (ortho_scale < 1.0f) ortho_scale = 1.0f;
        break;
      case Qt::Key_S:
        ortho_scale += scale_speed;
        break;
      case Qt::Key_A:
        camera_x += move_speed;
        break;
      case Qt::Key_D:
        camera_x -= move_speed;
        break;
      case Qt::Key_Q:
        camera_y -= move_speed;
        break;
      case Qt::Key_E:
        camera_y += move_speed;
        break;
    }
  }
  update();
}

void OpenGLViewer::focusInEvent(QFocusEvent *event) { setFocus(); }

void OpenGLViewer::set_file_path(const QString &file_path) {
  w_file_path = file_path;
  reload_model();
}

void OpenGLViewer::reset_rotation() {
  total_angle_x = 0.0f;
  total_angle_y = 0.0f;
  total_angle_z = 0.0f;
}

void OpenGLViewer::reload_model() {
  if (!w_file_path.isEmpty()) {
    QByteArray file_file_bytes = w_file_path.toUtf8();
    const char *filename = file_file_bytes.constData();
    reset_sliders();
    clear();
    parser(filename, &vertices, &vertex_count, &faces, &face_count);
    update();
  }
}

void OpenGLViewer::clear() {
  if (vertices) {
    free(vertices);
    vertices = nullptr;
  }
  if (faces) {
    for (int i = 0; i < face_count; ++i) {
      if (faces[i].v) {
        free(faces[i].v);
        faces[i].v = nullptr;
      }
    }
    free(faces);
    faces = nullptr;
  }
  vertex_count = 0;
  face_count = 0;
}

void OpenGLViewer::create_model() {
  for (int i = 0; i < face_count; ++i) {
    glBegin(GL_LINE_STRIP);
    for (int j = 0; j < faces[i].vertex_count; ++j) {
      int vertex_index = faces[i].v[j] - 1;
      glVertex3f(vertices[vertex_index].x, vertices[vertex_index].y,
                 vertices[vertex_index].z);
    }
    glEnd();
  }
  glColor3f(vertex_color.redF(), vertex_color.greenF(), vertex_color.blueF());
  for (int i = 0; i < vertex_count; ++i) {
    glPushMatrix();
    glTranslatef(vertices[i].x, vertices[i].y, vertices[i].z);
    if (vertex_shape == "Круг") {
      draw_circle(vertex_size);
    } else if (vertex_shape == "Квадрат") {
      draw_square(vertex_size);
    }
    glPopMatrix();
  }
  emit model_updated(vertex_count, face_count);
}

void OpenGLViewer::create_default_cube() {
  vertex_count = 8;
  face_count = 6;
  vertices = (Vertex *)malloc(vertex_count * sizeof(Vertex));
  vertices[0] = {+1.0, -1.0, +1.0};
  vertices[1] = {+1.0, -1.0, -1.0};
  vertices[2] = {+1.0, +1.0, -1.0};
  vertices[3] = {+1.0, +1.0, +1.0};
  vertices[4] = {-1.0, -1.0, -1.0};
  vertices[5] = {-1.0, -1.0, +1.0};
  vertices[6] = {-1.0, +1.0, +1.0};
  vertices[7] = {-1.0, +1.0, -1.0};
  faces = (Face *)malloc(face_count * sizeof(Face));
  for (int i = 0; i < face_count; ++i) {
    faces[i].vertex_count = 4;
    faces[i].v = (int *)malloc(faces[i].vertex_count * sizeof(int));
  }
  faces[0].v[0] = 1;
  faces[0].v[1] = 2;
  faces[0].v[2] = 3;
  faces[0].v[3] = 4;
  faces[1].v[0] = 5;
  faces[1].v[1] = 6;
  faces[1].v[2] = 7;
  faces[1].v[3] = 8;
  faces[2].v[0] = 2;
  faces[2].v[1] = 5;
  faces[2].v[2] = 8;
  faces[2].v[3] = 3;
  faces[3].v[0] = 6;
  faces[3].v[1] = 1;
  faces[3].v[2] = 4;
  faces[3].v[3] = 7;
  faces[4].v[0] = 5;
  faces[4].v[1] = 6;
  faces[4].v[2] = 1;
  faces[4].v[3] = 2;
  faces[5].v[0] = 4;
  faces[5].v[1] = 3;
  faces[5].v[2] = 8;
  faces[5].v[3] = 7;
  for (int i = 0; i < face_count; ++i) {
    glBegin(GL_LINE_STRIP);
    for (int j = 0; j < faces[i].vertex_count; ++j) {
      int vertex_index = faces[i].v[j] - 1;
      glVertex3f(vertices[vertex_index].x, vertices[vertex_index].y,
                 vertices[vertex_index].z);
    }
    glEnd();
  }
  emit model_updated(vertex_count, face_count);
}

void OpenGLViewer::set_translation(GLfloat value, GLchar axis) {
  float delta = 0.0f;
  switch (axis) {
    case 'x':
      delta = value - current_tx;
      move_model(vertices, vertex_count, delta, 0.0f, 0.0f);
      current_tx = value;
      break;
    case 'y':
      delta = value - current_ty;
      move_model(vertices, vertex_count, 0.0f, delta, 0.0f);
      current_ty = value;
      break;
    case 'z':
      delta = value - current_tz;
      move_model(vertices, vertex_count, 0.0f, 0.0f, delta);
      current_tz = value;
      break;
  }
  update();
}

void OpenGLViewer::draw_grid() {
  glBegin(GL_LINES);
  for (int i = -10; i <= 10; ++i) {
    glVertex3f((GLfloat)i, 0.0f, -10.0f);
    glVertex3f((GLfloat)i, 0.0f, 10.0f);
    glVertex3f(-10.0f, 0.0f, (GLfloat)i);
    glVertex3f(10.0f, 0.0f, (GLfloat)i);
  }
  glEnd();
}

void OpenGLViewer::reset_sliders() {
  if (sliders.pos_x) sliders.pos_x->setValue(0);
  if (sliders.pos_y) sliders.pos_y->setValue(0);
  if (sliders.pos_z) sliders.pos_z->setValue(0);
  if (sliders.rot_x) sliders.rot_x->setValue(360);
  if (sliders.rot_y) sliders.rot_y->setValue(360);
  if (sliders.rot_z) sliders.rot_z->setValue(360);
  if (sliders.scale) sliders.scale->setValue(50);
}

void OpenGLViewer::set_sliders(const Sliders &sliders) {
  this->sliders = sliders;
}

int OpenGLViewer::get_vertex_count() const { return vertex_count; }

int OpenGLViewer::get_face_count() const { return face_count; }

void OpenGLViewer::set_rotation(GLfloat slider_value, GLchar axis) {
  float angle = slider_value * 360.0f / 720.0f;
  static float total_angle_x = 0.0f;
  static float total_angle_y = 0.0f;
  static float total_angle_z = 0.0f;
  float delta_angle = angle - ((axis == 'x')   ? total_angle_x
                               : (axis == 'y') ? total_angle_y
                               : (axis == 'z') ? total_angle_z
                                               : 0.0f);
  float rotation_speed = 2.0f;

  switch (axis) {
    case 'x':
      total_angle_x = angle;
      break;
    case 'y':
      total_angle_y = angle;
      break;
    case 'z':
      total_angle_z = angle;
      break;
  }
  rotate(vertices, vertex_count,
         (axis == 'x') ? delta_angle * rotation_speed : 0.0f,
         (axis == 'y') ? delta_angle * rotation_speed : 0.0f,
         (axis == 'z') ? delta_angle * rotation_speed : 0.0f);

  update();
}

void OpenGLViewer::set_scaling(GLint slider_value) {
  GLfloat min_scale{0.1f};
  GLfloat max_scale{2.0f};
  GLfloat new_scale{min_scale +
                    (slider_value / 100.0f) * (max_scale - min_scale)};
  GLfloat scale_factor{new_scale / current_scale};
  GLfloat cx{0}, cy{0}, cz{0};
  find_model_center(vertices, vertex_count, cx, cy, cz);
  move_model(vertices, vertex_count, -cx, -cy, -cz);
  scale_vertices(vertices, vertex_count, scale_factor);
  move_model(vertices, vertex_count, cx, cy, cz);
  current_scale = new_scale;
  update();
}

void OpenGLViewer::find_model_center(Vertex *vertices, GLint vertex_count,
                                     GLfloat &cx, GLfloat &cy, GLfloat &cz) {
  if (vertex_count == 0) return;
  GLfloat min_x = vertices[0].x, max_x = vertices[0].x;
  GLfloat min_y = vertices[0].y, max_y = vertices[0].y;
  GLfloat min_z = vertices[0].z, max_z = vertices[0].z;
  for (int i = 1; i < vertex_count; ++i) {
    if (vertices[i].x < min_x) min_x = vertices[i].x;
    if (vertices[i].x > max_x) max_x = vertices[i].x;
    if (vertices[i].y < min_y) min_y = vertices[i].y;
    if (vertices[i].y > max_y) max_y = vertices[i].y;
    if (vertices[i].z < min_z) min_z = vertices[i].z;
    if (vertices[i].z > max_z) max_z = vertices[i].z;
  }
  cx = (min_x + max_x) / 2.0f;
  cy = (min_y + max_y) / 2.0f;
  cz = (min_z + max_z) / 2.0f;
}

void OpenGLViewer::set_projection(bool perspective) {
  if (perspective_view != perspective) {
    perspective_view = perspective;
    if (perspective_view) {
      camera_z = -ortho_scale;
    } else {
      ortho_scale = -camera_z;
    }
    update();
  }
}

void OpenGLViewer::change_background_color(const QColor &color) {
  background_color = color;
  update_background_color();
}

void OpenGLViewer::update_background_color() {
  QOpenGLContext *context = QOpenGLContext::currentContext();
  if (context) {
    QOpenGLFunctions *func = context->functions();
    GLfloat r = background_color.redF();
    GLfloat g = background_color.greenF();
    GLfloat b = background_color.blueF();
    GLfloat a = background_color.alphaF();
    func->glClearColor(r, g, b, a);
    update();
  }
}

void OpenGLViewer::set_edge_color(const QColor &color) {
  edge_color = color;
  update();
}

void OpenGLViewer::set_vertex_color(const QColor &color) {
  vertex_color = color;
  update();
}

void OpenGLViewer::set_edge_thickness(GLfloat thickness) {
  edge_thickness = thickness;
  update();
}

void OpenGLViewer::set_edge_dashed(bool dashed) {
  edge_dashed = dashed;
  update();
}

void OpenGLViewer::set_vertex_size(GLfloat size) {
  vertex_size = size;
  update();
}

void OpenGLViewer::set_vertex_shape(const QString &shape) {
  vertex_shape = shape;
  update();
}

void OpenGLViewer::draw_circle(GLfloat size) {
  int num_segments = 20;
  GLfloat angle_step = 2.0f * M_PI / num_segments;
  glBegin(GL_TRIANGLE_FAN);
  for (int i = 0; i <= num_segments; ++i) {
    GLfloat angle = i * angle_step;
    glVertex2f(size / 10 * cos(angle), size / 10 * sin(angle));
  }
  glEnd();
}

void OpenGLViewer::draw_square(GLfloat size) {
  GLfloat half_size = size / 10.0f;
  glBegin(GL_QUADS);
  glVertex2f(-half_size, -half_size);
  glVertex2f(half_size, -half_size);
  glVertex2f(half_size, half_size);
  glVertex2f(-half_size, half_size);
  glEnd();
}

QColor OpenGLViewer::get_edge_color() const { return edge_color; }

QColor OpenGLViewer::get_vertex_color() const { return vertex_color; }

QColor OpenGLViewer::get_background_color() const { return background_color; }

bool OpenGLViewer::get_projection() const { return perspective_view; }
