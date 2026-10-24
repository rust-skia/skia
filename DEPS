# This file manages the dependencies used by Skia developers for local, stand-alone Skia builds and
# Skia testing infrastructure. The versions specified by the commit hashes represent the revisions
# we happen to be currently testing. Skia provides no endorsement or recommendation on the revision
# to use for these libraries.
use_relative_paths = True

vars = {
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling different
  # dependencies without interference from each other.
  'infra_revision': '53958560a763f6d798b4fb80ed7576b2d5a45218',

  # ninja CIPD package version.
  # https://chrome-infra-packages.appspot.com/p/infra/3pp/tools/ninja
  'ninja_version': 'version:2@1.12.1.chromium.4',

  # googlefonts_testdata CIPD package version
  # https://chrome-infra-packages.appspot.com/p/chromium/third_party/googlefonts_testdata/
  'googlefonts_testdata_version': 'version:20230913',

  # Pre-built task drivers from this repo, used for CI.
  'task_drivers_revision': 'git_revision:a24a8c916deca7ed9ea7dd28c74c55de853c98b6',

  'checkout_agents_internal': False,
}

# If you modify this file, you will need to regenerate the Bazel version of this file (bazel/deps.bzl).
# To do so, run:
#     bazelisk run //bazel/deps_parser
#
# To apply the changes for the GN build, you will need to resync the git repositories using:
#     ./tools/git-sync-deps
deps = {
  "buildtools"                                   : "https://chromium.googlesource.com/chromium/src/buildtools.git@729495f2ffa69080907780591fa2a630b2556e98",
  "third_party/externals/brotli"                 : "https://skia.googlesource.com/external/github.com/google/brotli.git@cfa184a997ac19454d5a3ae97f6d600aac2445a6",
  "third_party/externals/d3d12allocator"         : "https://skia.googlesource.com/external/github.com/GPUOpen-LibrariesAndSDKs/D3D12MemoryAllocator.git@169895d529dfce00390a20e69c2f516066fe7a3b",
  # Dawn requires jinja2 and markupsafe for the code generator, abseil for string formatting,
  # and a few other depencencies.
  # When the Dawn revision is updated these should be updated from the Dawn DEPS as well.
  "third_party/externals/expat"                  : "https://chromium.googlesource.com/external/github.com/libexpat/libexpat.git@a76b1174c1ea52abb639a4c69d52b630a8a106a2",
  "third_party/externals/freetype"               : "https://chromium.googlesource.com/chromium/src/third_party/freetype2.git@891f904e2e7b4289f615598cd4b69832f87d2f3a",
  "third_party/externals/harfbuzz"               : "https://chromium.googlesource.com/external/github.com/harfbuzz/harfbuzz.git@f5efbbef3841bdcdc6427bde7c2971f62ac8c8f1",
  "third_party/externals/icu"                    : "https://chromium.googlesource.com/chromium/deps/icu.git@2a51536dfb3b448bdf3dae70c782ea2879c90904",
  "third_party/externals/libjpeg-turbo"          : "https://chromium.googlesource.com/chromium/deps/libjpeg_turbo.git@640f254ad0fa03f6b1f29f89b7dd9366f2f6e533",
  "third_party/externals/libpng"                 : "https://skia.googlesource.com/third_party/libpng.git@3061454d980de7d53608f594194cfac722721d2a",
  "third_party/externals/libwebp"                : "https://chromium.googlesource.com/webm/libwebp.git@b43b2caa710c0c997c066cb32c7fea1391fad70a",
  "third_party/externals/vulkanmemoryallocator"  : "https://chromium.googlesource.com/external/github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator@eb744ea7a2b17040121b4bbb4d6f9e8a77e3cae7",
  # vulkan-deps is a meta-repo containing several interdependent Khronos Vulkan repositories.
  # When the vulkan-deps revision is updated, those repos (spirv-*, vulkan-*) should be updated as well.
  "third_party/externals/spirv-cross"            : "https://chromium.googlesource.com/external/github.com/KhronosGroup/SPIRV-Cross@b8fcf307f1f347089e3c46eb4451d27f32ebc8d3",
  #"third_party/externals/v8"                     : "https://chromium.googlesource.com/v8/v8.git@5f1ae66d5634e43563b2d25ea652dfb94c31a3b4",
  "third_party/externals/wuffs"                  : "https://skia.googlesource.com/external/github.com/google/wuffs-mirror-release-c.git@7411f488fe2e2c205c3d3b3d28638b7356522930",
  "third_party/externals/zlib"                   : "https://chromium.googlesource.com/chromium/src/third_party/zlib@456ae730017b9b4bd3371927abc82627fd51feef",

  'bin': {
    'packages': [
      {
        'package': 'skia/tools/sk/${{platform}}',
        'version': 'git_revision:' + Var('infra_revision'),
      },
      {
        'package': 'infra/3pp/tools/ninja/${{platform}}',
        'version': Var('ninja_version'),
      }
    ],
    'dep_type': 'cipd',
  },

  'task_drivers': {
    'packages': [
      {
        'package': 'skia/tools/bazel_build/${{platform}}',
        'version': Var('task_drivers_revision'),
      },
    ],
    'dep_type': 'cipd',
    'condition': 'False',
  },

  'infra/skia-infra': {
    'url': 'https://skia.googlesource.com/buildbot.git@' + Var('infra_revision'),
    'condition': 'False',
  },

  'agents/shared': 'https://chromium.googlesource.com/chromium/agents/@e75efa515896f6bf1dea92eaffbcf8ee711a65d8',

  'agents/internal': {
    'url': 'https://chrome-internal.googlesource.com/chrome/agents-internal/@11c700b10e171091b4f0f3cf3bf95f13dee85c93',
    'condition': 'checkout_agents_internal',
  },
}
