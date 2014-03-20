angular.module('codepuppy').controller('StudentAssignmentCtrl', ['$scope',
    '$stateParams', '$fileUploader', '$http', '$sce',
    function($scope, $stateParams, $fileUploader, $http, $sce)
{
    $http({
        method: 'GET',
        url: '/assignments/' +
             $stateParams.assignmentID + '.json'
    })
    .success(function(data, status, headers, config) {
        if (data.name) {
            $scope.assignmentName = data.name;
        } else {
            $scope.assignmentName = 'Assignment';
        }

        if (data.pdf_url) {
            $scope.pdfHTML = $sce.trustAsHtml("<embed width=100% type='application/pdf' src='" + data.pdf_url + "' width=1024 height = 768>");
        } else {
            $scope.pdfURL = null;
        }
    });

    $scope.uploadPath = '/#/courses/' + $stateParams.courseID +
        '/assignments/' + $stateParams.assignmentID + '/submit';
}]);
