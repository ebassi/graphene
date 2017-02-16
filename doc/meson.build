subdir('xml')

private_headers = [
  'config.h',

  'graphene.h',
  'graphene-alloc-private.h',
  'graphene-bench-utils.h',
  'graphene-config.h',
  'graphene-line-segment-private.h',
  'graphene-macros.h',
  'graphene-private.h',
  'graphene-test-compat.h',
  'graphene-version-macros.h',
  'graphene-vectors-private.h',
]

html_images = [
  'rectangle-intersection.png',
  'rectangle-union.png',
  'rectangle.png',
  'triangle-barycentric.png',
]

glib_prefix = dependency('glib-2.0').get_pkgconfig_variable('prefix')
glib_docpath = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')
docpath = join_paths(graphene_datadir, 'gtk-doc', 'html')

gnome.gtkdoc('graphene',
             main_xml: 'graphene-docs.xml',
             src_dir: [ join_paths(meson.source_root(), 'src'), join_paths(meson.build_root(), 'src'), ],
             dependencies: graphene_dep,
             gobject_typesfile: 'graphene.types',
             scan_args: [
               '--rebuild-types',
               '--ignore-decorators=_GRAPHENE_PUBLIC',
               '--ignore-headers=' + ' '.join(private_headers),
             ],
             fixxref_args: [
               '--html-dir=@0@'.format(docpath),
               '--extra-dir=@0@'.format(join_paths(glib_docpath, 'glib')),
               '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gobject')),
             ],
             html_assets: html_images,
             install: true)
