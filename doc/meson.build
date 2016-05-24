gnome.gtkdoc('graphene',
  main_xml: 'graphene-docs.xml',
  src_dir: '@0@/src'.format(meson.source_root()),
  scan_args: [
    '--rebuild-types',
    '--ignore-decorators=_GRAPHENE_PUBLIC',
    '--ignore-headers=config.h graphene-config.h graphene-private.h graphene-alloc-private.h graphene-line-segment-private.h graphene-macros.h graphene-version-macros.h graphene-vectors-private.h graphene-bench-utils.h graphene-test-compat.h'
  ],
  install: true)


# XXX: Would be nice for gnome.gtkdoc() to do this for us
html_images = [
  'rectangle-intersection.png',
  'rectangle-union.png',
  'rectangle.png',
  'triangle-barycentric.png'
]

install_data('images', sources: html_images, install_dir: 'share/gtk-doc/html/graphene-1.0')
