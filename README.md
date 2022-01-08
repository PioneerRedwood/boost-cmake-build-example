# boost-cmake-build-example
- boost 내 라이브러리를 CMake로 프로젝트를 생성하며 CMake를 익히는 개인 프로젝트입니다.
- CMakeLists.txt 파일들은 cef, boost tools 등의 오픈소스 내에서 사용되는 예시들을 참고했습니다.

### BAN
boost-asio-network의 약자
- CMake에서 제공하는 boost 찾기 매크로 https://cmake.org/cmake/help/latest/module/FindBoost.html

### simple_server/client
- boost_asio daytime tutorial 참고, 저작권
- 위 서버, 클라이언트를 다른 디렉토리로 구분해서 제작

### ban_network
- [boost-asio-network](https://github.com/PioneerRedwood/boost-asio-network)에서 구현했던 서버, 클라이언트 가져오기
- 공통부분은 `common`으로 넣기
- client, server 빌드 시스템 생성기 완료
- 클라이언트 메시지 수신 부분 스레드 안전하게 다시 구현하기