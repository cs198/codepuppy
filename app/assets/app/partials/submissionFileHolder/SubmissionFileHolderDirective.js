angular.module('codepuppy').directive('submissionFileHolder', ['$timeout', function($timeout) {
  return {
    restrict: 'E',
    scope: {
      'file': '=file',
      'commentsReleased': '=commentsreleased', 
      'commentPermissions': '=commentpermissions'
    },

    link: function($scope) {
      $scope.$watch('comments', function() {
        $scope.$$childHead.getComments();
      });
      $scope.$watch('file', function() {
        $scope.$$childHead.getComments();
        if($scope.$$childHead.file !== undefined) {
          $scope.$$childHead.fileLines = $scope.$$childHead.file.data.split('\n');
        }
      });
    },

    templateUrl: '/assets/partials/submissionFileHolder/submissionFileHolder.html'
  };
}]);